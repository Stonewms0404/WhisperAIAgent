# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "D:/Programming Projects/WhisperAIAgent/build/include/piper/s/src/spdlog_external")
  file(MAKE_DIRECTORY "D:/Programming Projects/WhisperAIAgent/build/include/piper/s/src/spdlog_external")
endif()
file(MAKE_DIRECTORY
  "D:/Programming Projects/WhisperAIAgent/build/include/piper/s/src/spdlog_external-build"
  "D:/Programming Projects/WhisperAIAgent/build/include/piper/s"
  "D:/Programming Projects/WhisperAIAgent/build/include/piper/s/tmp"
  "D:/Programming Projects/WhisperAIAgent/build/include/piper/s/src/spdlog_external-stamp"
  "D:/Programming Projects/WhisperAIAgent/build/include/piper/s/src"
  "D:/Programming Projects/WhisperAIAgent/build/include/piper/s/src/spdlog_external-stamp"
)

set(configSubDirs Debug;Release;MinSizeRel;RelWithDebInfo)
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/Programming Projects/WhisperAIAgent/build/include/piper/s/src/spdlog_external-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/Programming Projects/WhisperAIAgent/build/include/piper/s/src/spdlog_external-stamp${cfgdir}") # cfgdir has leading slash
endif()
