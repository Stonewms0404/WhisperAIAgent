# Install script for directory: D:/Programming Projects/WhisperAIAgent/include/whisper.cpp/ggml

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "D:/Programming Projects/WhisperAIAgent/out/install/x64-Debug")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("D:/Programming Projects/WhisperAIAgent/out/build/x64-Debug/include/whisper.cpp/ggml/src/cmake_install.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Programming Projects/WhisperAIAgent/out/build/x64-Debug/include/whisper.cpp/ggml/src/ggml.lib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/Programming Projects/WhisperAIAgent/out/build/x64-Debug/bin/ggml.dll")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "D:/Programming Projects/WhisperAIAgent/include/whisper.cpp/ggml/include/ggml.h"
    "D:/Programming Projects/WhisperAIAgent/include/whisper.cpp/ggml/include/ggml-cpu.h"
    "D:/Programming Projects/WhisperAIAgent/include/whisper.cpp/ggml/include/ggml-alloc.h"
    "D:/Programming Projects/WhisperAIAgent/include/whisper.cpp/ggml/include/ggml-backend.h"
    "D:/Programming Projects/WhisperAIAgent/include/whisper.cpp/ggml/include/ggml-blas.h"
    "D:/Programming Projects/WhisperAIAgent/include/whisper.cpp/ggml/include/ggml-cann.h"
    "D:/Programming Projects/WhisperAIAgent/include/whisper.cpp/ggml/include/ggml-cpp.h"
    "D:/Programming Projects/WhisperAIAgent/include/whisper.cpp/ggml/include/ggml-cuda.h"
    "D:/Programming Projects/WhisperAIAgent/include/whisper.cpp/ggml/include/ggml-opt.h"
    "D:/Programming Projects/WhisperAIAgent/include/whisper.cpp/ggml/include/ggml-metal.h"
    "D:/Programming Projects/WhisperAIAgent/include/whisper.cpp/ggml/include/ggml-rpc.h"
    "D:/Programming Projects/WhisperAIAgent/include/whisper.cpp/ggml/include/ggml-sycl.h"
    "D:/Programming Projects/WhisperAIAgent/include/whisper.cpp/ggml/include/ggml-vulkan.h"
    "D:/Programming Projects/WhisperAIAgent/include/whisper.cpp/ggml/include/ggml-webgpu.h"
    "D:/Programming Projects/WhisperAIAgent/include/whisper.cpp/ggml/include/gguf.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Programming Projects/WhisperAIAgent/out/build/x64-Debug/include/whisper.cpp/ggml/src/ggml-base.lib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/Programming Projects/WhisperAIAgent/out/build/x64-Debug/bin/ggml-base.dll")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/ggml" TYPE FILE FILES
    "D:/Programming Projects/WhisperAIAgent/out/build/x64-Debug/include/whisper.cpp/ggml/ggml-config.cmake"
    "D:/Programming Projects/WhisperAIAgent/out/build/x64-Debug/include/whisper.cpp/ggml/ggml-version.cmake"
    )
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "D:/Programming Projects/WhisperAIAgent/out/build/x64-Debug/include/whisper.cpp/ggml/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
