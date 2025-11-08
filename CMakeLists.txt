cmake_minimum_required(VERSION 3.16)
project(WhisperAIAgent VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    add_compile_definitions(_AMD64_ _M_AMD64)
endif()
if (MSVC)
    add_compile_options(/Zc:__cplusplus /permissive- /bigobj /utf-8)
endif()

set(PIPER_ENABLE_CXX_API ON CACHE BOOL "" FORCE)
set(PIPER_GENERATED_ONNXRUNTIME_DIR ${CMAKE_SOURCE_DIR}/include/piper/build/pi/include)
set(PIPER_GENERATED_PHONEMIZE_DIR   ${CMAKE_SOURCE_DIR}/include/piper/build/p/src/piper_phonemize_external/src)

add_executable(WhisperAIAgent
    src/main.cpp
    src/parser.h
    src/parser.cpp
    src/command.h
    src/command.cpp
    src/llama_simple.h
    src/llama_simple.cpp

    include/whisper.cpp/examples/common.cpp
    include/whisper.cpp/examples/common-sdl.cpp
    include/whisper.cpp/examples/common-ggml.cpp
    include/whisper.cpp/examples/grammar-parser.cpp
)

# whisper
add_subdirectory(include/whisper.cpp)

# espeak-ng
add_subdirectory(include/espeak-ng)

# piper
set(PIPER_BUILD_ONNXRUNTIME ON CACHE BOOL "" FORCE)
set(PIPER_FETCH_ONNXRUNTIME ON CACHE BOOL "" FORCE)
set(PIPER_FETCH_ESPEAK_NG OFF CACHE BOOL "" FORCE)
set(PIPER_ENABLE_CXX_API ON CACHE BOOL "" FORCE)
#add_subdirectory(include/piper)

# llama (modern ggml / correct linking)
set(LLAMA_BLAS OFF CACHE BOOL "" FORCE)
set(LLAMA_CUDA OFF CACHE BOOL "" FORCE)
set(LLAMA_K_QUANTS ON CACHE BOOL "" FORCE)
add_subdirectory(include/llama.cpp)

# Ensure whisper's library links against llama + ggml so examples and helper code compile.
# The 'whisper' target is created by include/whisper.cpp CMakeLists.
# This must come *after* add_subdirectory(include/whisper.cpp) and after llama target exists.
if (TARGET whisper AND TARGET llama)
    message(STATUS "Linking whisper target against llama and ggml")
    target_link_libraries(whisper PRIVATE llama ggml)
endif()

target_include_directories(WhisperAIAgent PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include/SDL2/include
    ${CMAKE_SOURCE_DIR}/include/espeak-ng/src/libespeak-ng
    ${CMAKE_SOURCE_DIR}/include/espeak-ng/src/include
    ${SDL2_INCLUDE_DIRS}

    # Piper C++ API
    ${CMAKE_SOURCE_DIR}/include/piper/src/cpp
    include/piper/build/p/src/piper_phonemize_external/src
    
    # generated piper deps
    ${PIPER_GENERATED_ONNXRUNTIME_DIR}
    ${PIPER_GENERATED_PHONEMIZE_DIR}

    # espeak-ng headers
    ${CMAKE_SOURCE_DIR}/include/espeak-ng/src/include
    ${CMAKE_SOURCE_DIR}/include/espeak-ng/src/libespeak-ng

    # SDL2 headers
    ${CMAKE_SOURCE_DIR}/include/SDL2/include
)

    target_link_directories(WhisperAIAgent PRIVATE
        ${CMAKE_SOURCE_DIR}/include/piper/build/pi/lib
        ${CMAKE_SOURCE_DIR}/include/SDL2/lib/x64
    )

# platform specific
if (WIN32)
    target_link_libraries(WhisperAIAgent PRIVATE
        whisper
        llama
        espeak-ng
        ggml
        SDL2
        SDL2main
        ole32
        uuid
        onnxruntime
    )
elseif(APPLE)
    find_library(COREAUDIO_FRAMEWORK CoreAudio)
    find_library(AUDIOTOOLBOX_FRAMEWORK AudioToolbox)
    target_link_libraries(WhisperAIAgent PRIVATE whisper llama espeak-ng ggml SDL2 onnxruntime ${COREAUDIO_FRAMEWORK} ${AUDIOTOOLBOX_FRAMEWORK})
elseif(UNIX)
    find_package(ALSA REQUIRED)
    target_link_libraries(WhisperAIAgent PRIVATE whisper llama espeak-ng ggml SDL2 onnxruntime asound)
endif()


# =============================
# Post-build DLL / model copy
# =============================
# Copies whisper.cpp DLLs, SDL2.dll, and model into build folder
add_custom_command(TARGET WhisperAIAgent POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/include/whisper.cpp/ggml/build/bin/Release/ggml-cpu.dll"
        "${CMAKE_SOURCE_DIR}/build/Release/ggml-cpu.dll"

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/include/whisper.cpp/ggml/build/bin/Release/ggml-base.dll"
        "${CMAKE_SOURCE_DIR}/build/Release/ggml-base.dll"
        
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/include/whisper.cpp/ggml/build/bin/Release/ggml.dll"
        "${CMAKE_SOURCE_DIR}/build/Release/ggml.dll"

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/include/whisper.cpp/build/bin/Release/whisper.dll"
        "${CMAKE_SOURCE_DIR}/build/Release/whisper.dll"

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/include/whisper.cpp/models/ggml-base.bin"
        "${CMAKE_SOURCE_DIR}/build/Release/ggml-base.bin"

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/models/en_US-kathleen-low.onnx"
        "${CMAKE_SOURCE_DIR}/build/Release/en_US-kathleen-low.onnx"

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/include/llama.cpp/build/bin/Release/llama.dll"
        "${CMAKE_SOURCE_DIR}/build/Release/llama.dll"

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/include/SDL2/lib/x64/SDL2.dll"
        "${CMAKE_SOURCE_DIR}/build/Release/SDL2.dll"
)
