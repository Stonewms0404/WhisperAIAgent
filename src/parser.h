#pragma once
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <thread>

#include "llama.h"
#include "../include/llama.cpp/src/llama-model.h"
#include "llama_simple.h"


struct ArgvWrapper {
	std::vector<std::string> strings;
	std::vector<char*> cstrings;

	ArgvWrapper(std::vector<std::string> val) : strings(val) {
		// Convert to char* array
		cstrings.reserve(strings.size());
		for (auto& s : strings) {
			cstrings.push_back(s.data());
		}
	}

	char** data() { return cstrings.data(); }
	int size() { return static_cast<int>(cstrings.size()); }
};

struct Node {
	Node() {}
	Node(const std::string& value) : val(value) {};
	const std::string val;
	Node* next = nullptr;
};


class Parser {
public:
	Parser();
	~Parser();

	Node* m_commands = nullptr;
	std::vector<std::string> m_target_phrase;
	bool m_enabled = true;
	Llama_Simple* llama = nullptr;
	void ask_gemini(std::string prompt);
	bool parse_command(const std::string input_command);
	
private:
	std::filesystem::path path;

	static std::string tolowercase(std::string text);
	static int contains_at_location(const std::string text, const std::string target);
	static std::string get_after_substring(const std::string& str_a, const std::string& str_b);
	void reply_to_gemini(std::string prompt);
};
