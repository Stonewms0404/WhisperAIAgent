#include <stdio.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>
#include <conio.h>

#include "parser.h"
#include "command.h"

int main(int argc, char** argv){
	PiperTTS* piper = new PiperTTS();
	std::filesystem::path p = std::filesystem::current_path().parent_path();
	p /= "models";
	p /= "en_US-kathleen-low.onnx";

	piper->text_to_speech("Hello there!", "out.wav", p.string());

	if (false)
	{
	printf("Running Whisper AI Agent...\n");
			Parser* parser = new Parser();
		{
			std::vector<std::string> commands;
			std::filesystem::path p = std::filesystem::current_path().parent_path().parent_path();
			p /= "src/commands.txt";

			std::fstream f(p.string());
			if (f.is_open()) {
				printf("Found File\n");
				std::string line;
				while (getline(f, line)) {
					printf("Line: %s\n", line.c_str());
					commands.push_back(line);
				}
				f.close();
			}
			else {
				printf("File Not Found\n");
				return -1;
			}

			printf("Commands Length: %i\n", (int)commands.size());
			if (commands.size() == 0)
				return 0;
			parser->m_commands = new Node(commands[0]);
			Node* node = parser->m_commands;
			for (int i = 1; i < commands.size(); i++) {
				Node* temp = new Node(commands[i]);
				node->next = temp;
				node = node->next;
			}
		}

		parser->ask_gemini("Can you write a short story?");
		while (true) {}

		if (false) {
			printf("Commands:\n");
			Node* cur = parser->m_commands;
			while (cur != nullptr) {
				printf(cur->val.c_str());
				std::cout << std::endl;
				cur = cur->next;
			}

			std::cout << std::endl;

			ArgvWrapper argv_wrapper({ "./whisper-command", "-m", "ggml-base.bin", "-t", "8"});

			Command* command = new Command(parser);

			bool success = command->listen_to_input(argv_wrapper.size(), argv_wrapper.data()) == 0;
		}
	}

	return 0;
}