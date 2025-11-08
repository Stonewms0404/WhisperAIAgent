#include "pipertts.h"
#include <iostream>
#include <cstring> // memcpy
#include <cmath>

// Include Piper headers here. The exact header path depends on your layout.
// Adjust as needed.
//
// Example possibilities (you already added include/piper/src/cpp in your CMake):
//   #include <piper/piper.hpp>
//   #include "piper.hpp"
// For this code I include the commonly provided C++ header file name:
#include "piper.hpp"

// If the header lives under a different path (e.g. include/piper/src/cpp/piper.hpp)
// change the include above accordingly.

namespace PiperTTS {

    static const int DEFAULT_DEVICE_SAMPLE_RATE = 24000;

    // ------------------- helper: convert float32 -> int16 PCM -------------------
    static std::vector<int16_t> float_to_int16_le(const float* src, size_t n) {
        std::vector<int16_t> out;
        out.reserve(n);
        for (size_t i = 0; i < n; ++i) {
            float v = src[i];
            // clamp
            if (v > 1.0f) v = 1.0f;
            if (v < -1.0f) v = -1.0f;
            // convert
            int32_t s = static_cast<int32_t>(std::lround(v * 32767.0f));
            out.push_back(static_cast<int16_t>(s));
        }
        return out;
    }

    // ------------------- Engine implementation -------------------
    Engine& Engine::get() {
        static Engine instance;
        return instance;
    }

    Engine::Engine() = default;

    Engine::~Engine() {
        shutdown();
    }

    bool Engine::init(const Options& opts) {
        if (initialized_) return true;
        opts_ = opts;
        verbose_ = opts.verbose;

        // initialize piper runtime if required by Piper's API.
        // --- ADAPT: BELOW we assume Piper exposes a function/class to initialize runtime.
        // If your Piper build uses a different API, replace the block below with the appropriate initializer.
        try {
            if (verbose_) std::cerr << "[PiperTTS] initializing Piper runtime\n";
            // Example (adapt if not present):
            // piper::init_runtime(); // <- comment/uncomment depending on your piper
            // If Piper requires a model loader or global config, set it up here.
        }
        catch (const std::exception& e) {
            std::cerr << "[PiperTTS] Piper runtime init failed: " << e.what() << "\n";
            return false;
        }

        // initialize SDL for audio
        if (!init_sdl()) {
            std::cerr << "[PiperTTS] SDL init failed\n";
            // SDL optional? We'll still allow synthesize to work and return PCM.
        }

        initialized_ = true;
        return true;
    }

    void Engine::shutdown() {
        if (!initialized_) return;

        // deinit SDL
        deinit_sdl();

        // shutdown piper runtime if necessary
        try {
            if (verbose_) std::cerr << "[PiperTTS] shutting down Piper runtime\n";
            // piper::shutdown_runtime(); // adapt if required
        }
        catch (...) {}

        initialized_ = false;
    }

    bool Engine::init_sdl() {
        if (sdl_ok_) return true;
        if (SDL_Init(SDL_INIT_AUDIO) != 0) {
            std::cerr << "[PiperTTS] SDL_Init error: " << SDL_GetError() << "\n";
            sdl_ok_ = false;
            return false;
        }

        // open default audio device (we will queue int16 PCM)
        SDL_AudioSpec want;
        SDL_zero(want);
        want.freq = opts_.preferred_sample_rate > 0 ? opts_.preferred_sample_rate : DEFAULT_DEVICE_SAMPLE_RATE;
        want.format = AUDIO_S16SYS;
        want.channels = 1;
        want.samples = 4096;

        SDL_AudioSpec have;
        SDL_zero(have);

        device_id_ = SDL_OpenAudioDevice(nullptr, 0, &want, &have, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
        if (device_id_ == 0) {
            std::cerr << "[PiperTTS] SDL_OpenAudioDevice failed: " << SDL_GetError() << "\n";
            sdl_ok_ = false;
            return false;
        }

        // start playback paused=false so queueing will play
        SDL_PauseAudioDevice(device_id_, 0);
        if (verbose_) {
            std::cerr << "[PiperTTS] SDL opened device at " << have.freq << " Hz, channels=" << int(have.channels) << "\n";
        }
        sdl_ok_ = true;
        return true;
    }

    void Engine::deinit_sdl() {
        if (!sdl_ok_) return;
        SDL_PauseAudioDevice(device_id_, 1);
        SDL_CloseAudioDevice(device_id_);
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
        sdl_ok_ = false;
        device_id_ = 0;
    }

    std::optional<PCM> Engine::synthesize(const std::string& text) {
        if (!initialized_) {
            std::cerr << "[PiperTTS] synthesize called before init()\n";
            return {};
        }

        // Prepare config for Piper. Adjust the fields if your local Piper API differs.
        piper::SynthesisConfig sconf;
        sconf.sampleRate = opts_.preferred_sample_rate > 0 ? opts_.preferred_sample_rate : DEFAULT_DEVICE_SAMPLE_RATE;
        // If your piper::SynthesisConfig has different names, change accordingly.
        // e.g. sconf.sample_rate or sconf.sampleRate

        // Attempt to synthesize using the piper API.
        // --- IMPORTANT: The piper function below is a placeholder name.
        // Replace `piper::synthesize` with the concrete function from your Piper build:
        //
        //    bool piper::synthesize(const std::string &modelPath,
        //                          const std::string &text,
        //                          const piper::SynthesisConfig &cfg,
        //                          std::vector<float> &out_pcm,
        //                          int &out_sample_rate);
        //
        // Many Piper builds expose different function signatures. If that is the case,
        // adapt this block to call your Piper API and fill pcm.samples & pcm.sample_rate.
        //
        std::vector<float> float_pcm;
        int out_rate = sconf.sampleRate;

        try {
            if (verbose_) std::cerr << "[PiperTTS] calling Piper synth for text size " << text.size() << "\n";

            // ====== ADAPT THIS CALL TO YOUR PIPER API ======
            // Example assumed API:
            // bool ok = piper::synthesize(opts_.voice_model_path, text, sconf, float_pcm, out_rate);
            //
            // If your Piper exposes an object/class:
            // piper::Piper p(opts_.voice_model_path, ...);
            // p.synthesize(text, sconf, float_pcm, out_rate);
            //
            // If there is no such function, implement the correct call here.
            //
            bool ok = false;
#if 1
            // Attempt to call a commonly used signature if available.
            // This will compile only if your Piper headers expose the matching function.
            ok = piper::synthesize(opts_.voice_model_path, text, sconf, float_pcm, out_rate);
#else
            // If your piper API is different, replace the above call with the correct one.
#endif
            if (!ok) {
                std::cerr << "[PiperTTS] Piper synthesis returned false\n";
                return {};
            }
        }
        catch (const std::exception& e) {
            std::cerr << "[PiperTTS] exception during Piper synth: " << e.what() << "\n";
            return {};
        }
        catch (...) {
            std::cerr << "[PiperTTS] unknown exception during Piper synth\n";
            return {};
        }

        PCM pcm;
        pcm.samples = std::move(float_pcm);
        pcm.sample_rate = out_rate > 0 ? out_rate : sconf.sampleRate;
        pcm.channels = 1;
        if (pcm.sample_rate == 0) pcm.sample_rate = DEFAULT_DEVICE_SAMPLE_RATE;

        if (verbose_) std::cerr << "[PiperTTS] synthesize returned " << pcm.samples.size() << " samples @ " << pcm.sample_rate << " Hz\n";

        return pcm;
    }

    bool Engine::queue_float_pcm_to_sdl(const float* data, size_t n_samples, int sample_rate, int channels) {
        if (!sdl_ok_) {
            if (!init_sdl()) return false;
        }

        // Convert float->int16
        auto ints = float_to_int16_le(data, n_samples * channels);

        const uint8_t* bytes = reinterpret_cast<const uint8_t*>(ints.data());
        size_t byte_count = ints.size() * sizeof(int16_t);

        std::lock_guard<std::mutex> lk(audio_mutex_);
        // SDL_QueueAudio will copy data.
        if (SDL_QueueAudio(device_id_, bytes, static_cast<uint32_t>(byte_count)) != 0) {
            std::cerr << "[PiperTTS] SDL_QueueAudio failed: " << SDL_GetError() << "\n";
            return false;
        }

        return true;
    }

    bool Engine::play_pcm(const PCM& pcm, int queue_ms) {
        if (pcm.samples.empty()) return false;
        if (!sdl_ok_) {
            if (!init_sdl()) return false;
        }

        // If device sample rate differs, SDL will resample if allowed, but many builds don't.
        // For reliability, we assume device opened at the same samplerate as pcm.sample_rate.
        // If not, consider implementing a resampler (e.g. libsamplerate) or re-open SDL device with pcm.sample_rate.

        bool ok = queue_float_pcm_to_sdl(pcm.samples.data(), pcm.samples.size(), pcm.sample_rate, pcm.channels);
        if (!ok) return false;

        // Optionally block until queue emptied or for queue_ms milliseconds. We'll sleep a small amount to allow playback.
        // A non-blocking approach (preferred for token-by-token) is to not wait here.
        SDL_Delay(std::max(20, queue_ms)); // minimal delay to let device play a bit
        return true;
    }

    bool Engine::synth_and_play(const std::string& text) {
        auto pcm_opt = synthesize(text);
        if (!pcm_opt) return false;
        return play_pcm(*pcm_opt);
    }

} // namespace PiperTTS