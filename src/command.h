#pragma once

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "parser.h"
#include "../include/whisper.cpp/examples/common-sdl.h"
#include "../include/whisper.cpp/examples/common.h"
#include "whisper.h"
#include "../include/whisper.cpp/examples/grammar-parser.h"

// command-line parameters
struct whisper_params {
    int32_t n_threads = std::min(4, (int32_t)std::thread::hardware_concurrency());
    int32_t prompt_ms = 5000;
    int32_t command_ms = 8000;
    int32_t capture_id = -1;
    int32_t max_tokens = 32;
    int32_t audio_ctx = 0;

    float vad_thold = 0.6f;
    float freq_thold = 100.0f;

    float grammar_penalty = 100.0f;

    grammar_parser::parse_state grammar_parsed;

    bool translate = false;
    bool print_special = false;
    bool print_energy = false;
    bool no_timestamps = true;
    bool use_gpu = true;
    bool flash_attn = true;

    std::string language = "en";
    std::string model = "models/ggml-base.en.bin";
    std::string fname_out;
    std::string commands;
    std::string prompt;
    std::string context;
    std::string grammar;

    // A regular expression that matches tokens to suppress
    std::string suppress_regex;
};

class Command {
public:
	Command(Parser* parser) : m_parser(parser) {};
	int listen_to_input(int argc, char** argv);
private:
	Parser* m_parser;
};