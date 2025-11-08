#pragma once

#include <stdio>
#include <string>
#include <vector>
#include <mutex>
#include <memory>
#include <optional>

// SDL2 (for playback)
#include <SDL.h>

namespace PiperTTS {

    struct Options {
        // Model path (onnx/vits model). Provide full path or relative to working dir.
        std::string voice_model_path;

        // Desired sample rate for playback. Piper models often use 22050 or 24000.
        // If zero, the model/sample will decide and the returned vector will include its sample_rate as out param.
        int preferred_sample_rate = 24000;

        // If true, print debug logging.
        bool verbose = false;
    };

    struct PCM {
        std::vector<float> samples; // interleaved if channels > 1 (we keep mono by default)
        int sample_rate = 0;
        int channels = 1;
    };

    class Engine {
    public:
        // singleton accessor
        static Engine& get();

        // initialize engine (call once). Returns false on failure.
        bool init(const Options& opts);

        // Synthesize text -> PCM. Returns empty optional on failure.
        // This does NOT write a file.
        std::optional<PCM> synthesize(const std::string& text);

        // Play PCM immediately on default audio device (blocking until buffer queued) -- returns false on error.
        bool play_pcm(const PCM& pcm, int queue_ms = 100);

        // Convenience: synthesize and play (blocking play) in one call.
        bool synth_and_play(const std::string& text);

        // Shutdown and free resources.
        void shutdown();

        // debug helper
        void set_verbose(bool v) { verbose_ = v; }

    private:
        Engine();
        ~Engine();
        Engine(const Engine&) = delete;
        Engine& operator=(const Engine&) = delete;

        // internal helpers
        bool init_sdl();
        void deinit_sdl();
        bool queue_float_pcm_to_sdl(const float* data, size_t n_samples, int sample_rate, int channels);

    private:
        Options opts_;
        bool initialized_ = false;
        bool sdl_ok_ = false;
        bool verbose_ = false;

        // SDL device handle and lock
        SDL_AudioDeviceID device_id_ = 0;
        std::mutex audio_mutex_;
    };