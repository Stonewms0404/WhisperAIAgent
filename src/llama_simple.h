#include "../include/llama.cpp/include/llama.h"
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>


class Llama_Simple {
public:
	Llama_Simple(std::string ai_path);
	~Llama_Simple();
	std::string simple_response(int arg_c, char** arg_v);

private:
	std::string piper_model_location;

	const llama_vocab* vocab = nullptr;
	llama_model* model = nullptr;
	llama_context* ctx = nullptr;
	llama_sampler* smpl = nullptr;
    std::string ai_model_path;    // path to the model gguf file
    int ngl = 99;    // number of layers to offload to the GPU
    int n_predict = 32;    // number of tokens to predict
};
