#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "parser.h"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <combaseapi.h>
#elif defined(__APPLE__)
#include <CoreAudio/CoreAudio.h>
#include <AudioToolbox/AudioServices.h>
#include <ApplicationServices/ApplicationServices.h>
#include <cstdlib>


#elif defined(__linux__)
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#include <alsa/asoundlib.h>
#endif

Parser::Parser() {
	// Create the m_target_phrase vector of possible misinterpretations of the target phrase
	m_target_phrase.push_back("okay whisper");
	m_target_phrase.push_back("okay, whisper");
	m_target_phrase.push_back("ok whisper");
	m_target_phrase.push_back("ok, whisper");
	
	path = std::filesystem::current_path().parent_path().parent_path();
	path /= "models";
	path /= "gemma-3-4b-it-Q2_K_L.gguf";

	std::filesystem::path p = std::filesystem::current_path().parent_path();
	p /= "models";
	p /= "en_US-kathleen-low.onnx";

	llama = new Llama_Simple(path.string(), p.string());
}

Parser::~Parser() {
	// clean up m_commands pointer(s)
	Node* temp = m_commands;
	while (temp != nullptr) {
		Node* next = temp->next;
		delete temp;
		temp = next;
	}

	delete llama;
}

std::string Parser::tolowercase(std::string text) {
	std::transform(text.begin(), text.end(), text.begin(),
		[](unsigned char c) { return std::tolower(c); });
	return text;
}

int Parser::contains_at_location(const std::string text, const std::string target) {
	size_t pos = text.find(target);  // std::string::find returns npos if not found
	if (pos == std::string::npos)
		return -1;  // not found
	return static_cast<int>(pos);    // convert size_t to int
}

std::string Parser::get_after_substring(const std::string& str_a, const std::string& str_b) {
	int index = Parser::contains_at_location(str_a, str_b);
	if (index == -1) return "";
	int start = index + str_b.length();
	if (start >= str_a.length()) return "";
	return str_a.substr(start);
}

bool set_volume_percent(int percent) {
	if (percent < 0) percent = 0;
	if (percent > 100) percent = 100;

#if defined(_WIN32) || defined(_WIN64)
	HRESULT hr;
	CoInitialize(nullptr);

	IMMDeviceEnumerator* pEnumerator = nullptr;
	hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL,
		__uuidof(IMMDeviceEnumerator), (void**)&pEnumerator);
	if (FAILED(hr)) return false;

	IMMDevice* pDevice = nullptr;
	hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);
	if (FAILED(hr)) { pEnumerator->Release(); return false; }

	IAudioEndpointVolume* pVolume = nullptr;
	hr = pDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, nullptr, (void**)&pVolume);
	if (FAILED(hr)) { pDevice->Release(); pEnumerator->Release(); return false; }

	float level = percent / 100.0f;
	hr = pVolume->SetMasterVolumeLevelScalar(level, nullptr);

	pVolume->Release();
	pDevice->Release();
	pEnumerator->Release();
	CoUninitialize();

	return SUCCEEDED(hr);

#elif defined(__APPLE__)
	AudioDeviceID device;
	UInt32 size = sizeof(device);
	AudioObjectPropertyAddress pa = {
		kAudioHardwarePropertyDefaultOutputDevice,
		kAudioObjectPropertyScopeGlobal,
		kAudioObjectPropertyElementMaster
	};
	if (AudioObjectGetPropertyData(kAudioObjectSystemObject, &pa, 0, nullptr, &size, &device) != noErr) return false;

	Float32 flevel = percent / 100.0f;
	AudioObjectPropertyAddress volAddr = {
		kAudioDevicePropertyVolumeScalar,
		kAudioDevicePropertyScopeOutput,
		kAudioObjectPropertyElementMaster
	};
	return AudioObjectSetPropertyData(device, &volAddr, 0, nullptr, sizeof(flevel), &flevel) == noErr;

#elif defined(__linux__)
	long min, max;
	snd_mixer_t* handle;
	snd_mixer_selem_id_t* sid;
	const char* card = "default";
	const char* selem_name = "Master";

	if (snd_mixer_open(&handle, 0) < 0) return false;
	if (snd_mixer_attach(handle, card) < 0) { snd_mixer_close(handle); return false; }
	if (snd_mixer_selem_register(handle, nullptr, nullptr) < 0) { snd_mixer_close(handle); return false; }
	if (snd_mixer_load(handle) < 0) { snd_mixer_close(handle); return false; }

	snd_mixer_selem_id_malloc(&sid);
	snd_mixer_selem_id_set_index(sid, 0);
	snd_mixer_selem_id_set_name(sid, selem_name);
	snd_mixer_elem_t* elem = snd_mixer_find_selem(handle, sid);
	snd_mixer_selem_get_playback_volume_range(elem, &min, &max);
	long vol = min + (percent * (max - min) / 100);
	snd_mixer_selem_set_playback_volume_all(elem, vol);

	snd_mixer_selem_id_free(sid);
	snd_mixer_close(handle);
	return true;

#else
	return false; // Unsupported platform
#endif
}

#ifdef WIN32
void simulate_button_press(DWORD button) {
	INPUT input[2] = {};

	// Key press
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = button;

	// Key release
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = button;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;

	// Send both events
	SendInput(2, input, sizeof(INPUT));
}
#elif __linux__
void simulate_button_press(std::string button) {
	// Linux: Use X11 + XTest to send XF86AudioPlay key event
	Display* display = XOpenDisplay(NULL);
	if (display == NULL) {
		std::cerr << "Cannot open X display.\n";
		return;
	}

	KeyCode keycode = XKeysymToKeycode(display, XStringToKeysym(button.c_str()));
	if (keycode == 0) {
		std::cerr << "XF86AudioPlay key not found.\n";
		XCloseDisplay(display);
		return;
	}

	XTestFakeKeyEvent(display, keycode, True, CurrentTime);
	XTestFakeKeyEvent(display, keycode, False, CurrentTime);
	XFlush(display);
	XCloseDisplay(display);
}
#elif __APPLE__
void simulate_button_press(int button) {
	// macOS: Use HID system events to send play/pause (NX_KEYTYPE_PLAY)
	// Requires accessibility permissions
	CGEventRef eventDown = CGEventCreateKeyboardEvent(NULL, (CGKeyCode)0, true);
	CGEventSetIntegerValueField(eventDown, kCGKeyboardEventKeycode, button);
	CGEventPost(kCGHIDEventTap, eventDown);
	CFRelease(eventDown);
}
#endif

void Parser::ask_gemini(std::string prompt) {
	std::cout << "Asking Gemma: \"" << prompt << "\"\n";
	ArgvWrapper argv_wrapper({ "./llama", "-simple", "-m", path.string(), prompt });
	
	std::string out = llama->simple_response(argv_wrapper.size(), argv_wrapper.data());
	std::cout << std::endl << out << std::endl << std::endl;
}

void Parser::reply_to_gemini(std::string prompt) {
}


bool Parser::parse_command(const std::string input_command) {
	printf("Input String: \"%s\"\n", input_command.c_str());
	const std::string lower_command = tolowercase(input_command);
	std::string command_str;
	bool target_phrase_detected = false;
	for (int i = 0; i < m_target_phrase.size(); i++) {
		command_str = get_after_substring(lower_command, m_target_phrase[i]);
		target_phrase_detected = command_str != "";
		if (target_phrase_detected)
			break;
	}
	printf("Command String: \"%s\"\n", command_str.c_str());
	if (!target_phrase_detected) return false;

	bool command_phrase_detected = false;
	int index = 0;
	int i = -1;
	Node* node = m_commands;
	std::string action;
	while (node->next != nullptr) {
		i++;
		action = get_after_substring(command_str, node->val);
		command_phrase_detected = action != "";
		if (command_phrase_detected) {
			index = i;
			break;
		}
		node = node->next;
	}
	if (!command_phrase_detected) return false;
	if (!m_enabled && index != 1) {
		printf("Whisper is Disabled.\n");
		return false;
	}

	int volume_percent = 0;
	std::string volume_str;
	switch (index) {
	case 0: // Enable
		printf("Whisper set to Enable.\n");
		m_enabled = true;
		break;
	case 1: // Disable
		printf("Whisper set to Disable.\n");
		m_enabled = false;
		break;
	case 2: // Quit
		printf("Quit Command.\n");
		exit(0);
	case 3: // Stop
		printf("Stop Command.\n");
#if _WIN32
		simulate_button_press(VK_MEDIA_PLAY_PAUSE);
#elif __linux__
		simulate_button_press("XF86AudioPlay");
#elif __APPLE__
		simulate_button_press(NX_KEYTYPE_PLAY);
#endif
		break;
	case 4: // Music
	case 5: // Play
	case 6: // Resume
	case 7: // Pause
		printf("Play/Pause Pressed.\n");
#if _WIN32
		simulate_button_press(VK_MEDIA_PLAY_PAUSE);
#elif __linux__
		simulate_button_press("XF86AudioPlay");
#elif __APPLE__
		simulate_button_press(NX_KEYTYPE_PLAY);
#endif
		break;
	case 8: // Next
	case 9: // Skip
		printf("Next Pressed.\n");
#if _WIN32
		simulate_button_press(VK_MEDIA_NEXT_TRACK);
#elif __linux__
		simulate_button_press("XF86AudioNext");
#elif __APPLE__
		simulate_button_press(NX_KEYTYPE_NEXT);
#endif
		break;
	case 10: // Previous
	case 11: // Replay
		printf("Previous Pressed.\n");
#if _WIN32
		simulate_button_press(VK_MEDIA_PREV_TRACK);
#elif __linux__
		simulate_button_press("XF86AudioPrev");
#elif __APPLE__
		simulate_button_press(NX_KEYTYPE_PREVIOUS);
#endif
		break;
	case 12: // Mute Volume
	case 13: // Unmute Volume
		printf("Mute/Unmute Pressed.\n");
#if _WIN32
		simulate_button_press(VK_VOLUME_MUTE);
#elif __linux__
		simulate_button_press("XF86AudioMute");
#elif __APPLE__
		simulate_button_press(NX_KEYTYPE_MUTE);
#endif
		break;
	case 14: // Volume Up
		printf("Volume Up Pressed.\n");
#if _WIN32
		simulate_button_press(VK_VOLUME_UP);
#elif __linux__
		simulate_button_press("XF86AudioRaiseVolume");
#elif __APPLE__
		simulate_button_press(NX_KEYTYPE_SOUND_UP);
#endif
		break;
	case 15: // Volume Down
		printf("Volume Down Pressed.\n");
#if _WIN32
		simulate_button_press(VK_VOLUME_DOWN);
#elif __linux__
		simulate_button_press("XF86AudioLowerVolume");
#elif __APPLE__
		simulate_button_press(NX_KEYTYPE_SOUND_DOWN);
#endif
		break;
	case 16: // Volume Control
		for (int i = 0; i < action.size(); i++) {
			if (isdigit(action[i]))
				volume_str.push_back(action[i]);
			else if (volume_str.length() != 0)
				break;
		}
		if (volume_str.length() == 0)
			return false;
		volume_percent = std::stoi(volume_str);
		set_volume_percent(volume_percent);
		break;
	case 17: // Ask Gemini
		std::cout << "Asking Gemma: \"" << action << "\"\n";
		ask_gemini(action);
		break;
	case 18: // Reply to Gemini
		reply_to_gemini(action);
		break;
	default: 
		return false;
	}
	return true;
}
