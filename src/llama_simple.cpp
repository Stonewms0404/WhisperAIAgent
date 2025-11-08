#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>

#include "llama.h"
#include "llama_simple.h"
#include "piper_tts.h"
#include "piper.hpp"
#include <synchapi.h>

Llama_Simple::Llama_Simple(std::string ai_path, std::string voice_path)
    : ai_model_path(ai_path), piper_model_location(voice_path) {
    // load dynamic backends
    ggml_backend_load_all();
    llama_backend_init();

    // initialize the model
    llama_model_params model_params = llama_model_default_params();
    model_params.n_gpu_layers = ngl;

    model = llama_model_load_from_file(ai_model_path.c_str(), model_params);

    if (model == NULL) {
        fprintf(stderr, "%s: error: unable to load model\n", __func__);
        Sleep(5000); Sleep(5000); exit(1);
    }

    vocab = llama_model_get_vocab(model);

    auto sparams = llama_sampler_chain_default_params();
    sparams.no_perf = false;
    smpl = llama_sampler_chain_init(sparams);

    llama_sampler_chain_add(smpl, llama_sampler_init_top_k(40));
    llama_sampler_chain_add(smpl, llama_sampler_init_top_p(0.9f, 1));
    llama_sampler_chain_add(smpl, llama_sampler_init_min_p(0.10f, 1));
    llama_sampler_chain_add(smpl, llama_sampler_init_temp(0.8f));
}

Llama_Simple::~Llama_Simple() {
    llama_sampler_free(smpl);
    llama_free(ctx);
    llama_model_free(model);
    llama_backend_free();
}

std::string Llama_Simple::simple_response(int arg_c, char** arg_v) {
    // ----------------------------
    // Parse command-line arguments
    // ----------------------------
    std::string user_string;
    int i = 1;
    for (; i < arg_c; i++) {
        if (strcmp(arg_v[i], "-m") == 0 && i + 1 < arg_c) {
            ai_model_path = arg_v[++i];
        }
        else if (strcmp(arg_v[i], "-n") == 0 && i + 1 < arg_c) {
        }
        else if (strcmp(arg_v[i], "-ngl") == 0 && i + 1 < arg_c) {
            ngl = std::stoi(arg_v[++i]);
        }
        else {
            break; // start of user prompt
        }
    }
    n_predict = 512;

    if (ai_model_path.empty()) {
        Sleep(5000);
        exit(1);
    }

    for (; i < arg_c; i++) {
        if (!user_string.empty()) user_string += " ";
        user_string += arg_v[i];
    }

    // ----------------------------
    // Prepare prompt
    // ----------------------------
    std::string prompt =
        "<|im_start|>user\n" +
        user_string + "\n<|im_end|>\n<|im_start|>assistant\n";

    // estimate max tokens (overallocate to be safe)
    size_t max_tokens = prompt.size() * 4; // worst-case UTF-8 -> tokens
    std::vector<llama_token> tmp_tokens(max_tokens);

    // tokenize
    int n_prompt = llama_tokenize(vocab, prompt.c_str(), prompt.size(), tmp_tokens.data(), tmp_tokens.size(), true, true);
    if (n_prompt <= 0) {
        fprintf(stderr, "failed to count prompt tokens\n");
        exit(1);
    }

    // now you can copy to exact-size vector
    std::vector<llama_token> prompt_tokens(tmp_tokens.begin(), tmp_tokens.begin() + n_prompt);

    // ----------------------------
    // Initialize context
    // ----------------------------
    llama_context_params ctx_params = llama_context_default_params();
    ctx_params.n_ctx = 4096; // adjust to your model's context size
    ctx_params.n_batch = n_predict;
    ctx_params.no_perf = false;

    ctx = llama_init_from_model(model, ctx_params);
    if (!ctx) {
        fprintf(stderr, "failed to create llama_context\n");
        Sleep(5000); exit(1);
    }

    // Tokenize prompt into a vector
    int n_actual = llama_tokenize(vocab, prompt.c_str(), prompt.size(), prompt_tokens.data(), n_prompt, true, true);
    if (n_actual != n_prompt) {
        fprintf(stderr, "tokenize mismatch: %d != %d\n", n_actual, n_prompt);
        Sleep(5000); exit(1);
    }

    // ----------------------------
    // Print prompt
    // ----------------------------
    for (auto id : prompt_tokens) {
        char buf[128];
        int n = llama_token_to_piece(vocab, id, buf, sizeof(buf), 0, true);
        if (n < 0) { fprintf(stderr, "failed to convert token to piece\n"); Sleep(5000); exit(1); }
        printf("%.*s", n, buf);
    }
    fflush(stdout);

    // ----------------------------
    // Prepare batch for prompt
    // ----------------------------
    llama_batch batch = llama_batch_get_one(prompt_tokens.data(), prompt_tokens.size());

    if (llama_model_has_encoder(model)) {
        if (llama_encode(ctx, batch)) {
            fprintf(stderr, "failed to encode prompt\n");
            Sleep(5000); exit(1);
        }
        llama_token decoder_start_token_id = llama_model_decoder_start_token(model);
        if (decoder_start_token_id == LLAMA_TOKEN_NULL)
            decoder_start_token_id = llama_vocab_bos(vocab);
        batch = llama_batch_get_one(&decoder_start_token_id, 1);
    }

    // ----------------------------
    // Initialize sampler
    // ----------------------------
    llama_sampler_chain_add(smpl, llama_sampler_init_greedy());

    // ----------------------------
    // Main generation loop
    // ----------------------------
    std::cout << "\nStarting printing reply:\n";

    std::string ret_string;
    std::string sentence_buffer;
    int n_tokens_generated = 0;

    while (n_tokens_generated < n_predict) {
        if (llama_decode(ctx, batch)) break;

        int new_token_id = llama_sampler_sample(smpl, ctx, -1);
        if (llama_vocab_is_eog(vocab, new_token_id)) break;

        char buf[128];
        int n = llama_token_to_piece(vocab, new_token_id, buf, sizeof(buf), 0, true);
        if (n < 0) break;

        std::string token(buf, n);
        ret_string += token;
        sentence_buffer += token;

        // Detect end of sentence for TTS
        if (token.find('.') != std::string::npos ||
            token.find('!') != std::string::npos ||
            token.find('?') != std::string::npos ||
            token.find('\n') != std::string::npos) {

            if (!sentence_buffer.empty()) {
                piper->text_to_speech(sentence_buffer, "out.wav", piper_model_location);
                sentence_buffer.clear();

                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
        }

        // Next token
        batch = llama_batch_get_one(&new_token_id, 1);
        n_tokens_generated++;
    }

    // after loop flush remainder:
    if (!sentence_buffer.empty()) {
        piper->text_to_speech(sentence_buffer, "out.wav", piper_model_location);
        sentence_buffer.clear();
    }

    std::cout << "\nReply finished\n";

    return ret_string;
}