# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "D:/Programming Projects/WhisperAIAgent/build/include/piper/p/src/piper_phonemize_external-build/e/src/espeak_ng_external-build/_deps/sonic-git-src")
  file(MAKE_DIRECTORY "D:/Programming Projects/WhisperAIAgent/build/include/piper/p/src/piper_phonemize_external-build/e/src/espeak_ng_external-build/_deps/sonic-git-src")
endif()
file(MAKE_DIRECTORY
  "D:/Programming Projects/WhisperAIAgent/build/include/piper/p/src/piper_phonemize_external-build/e/src/espeak_ng_external-build/_deps/sonic-git-build"
  "D:/Programming Projects/WhisperAIAgent/build/include/piper/p/src/piper_phonemize_external-build/e/src/espeak_ng_external-build/_deps/sonic-git-subbuild/sonic-git-populate-prefix"
  "D:/Programming Projects/WhisperAIAgent/build/include/piper/p/src/piper_phonemize_external-build/e/src/espeak_ng_external-build/_deps/sonic-git-subbuild/sonic-git-populate-prefix/tmp"
  "D:/Programming Projects/WhisperAIAgent/build/include/piper/p/src/piper_phonemize_external-build/e/src/espeak_ng_external-build/_deps/sonic-git-subbuild/sonic-git-populate-prefix/src/sonic-git-populate-stamp"
  "D:/Programming Projects/WhisperAIAgent/build/include/piper/p/src/piper_phonemize_external-build/e/src/espeak_ng_external-build/_deps/sonic-git-subbuild/sonic-git-populate-prefix/src"
  "D:/Programming Projects/WhisperAIAgent/build/include/piper/p/src/piper_phonemize_external-build/e/src/espeak_ng_external-build/_deps/sonic-git-subbuild/sonic-git-populate-prefix/src/sonic-git-populate-stamp"
)

set(configSubDirs Debug)
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/Programming Projects/WhisperAIAgent/build/include/piper/p/src/piper_phonemize_external-build/e/src/espeak_ng_external-build/_deps/sonic-git-subbuild/sonic-git-populate-prefix/src/sonic-git-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/Programming Projects/WhisperAIAgent/build/include/piper/p/src/piper_phonemize_external-build/e/src/espeak_ng_external-build/_deps/sonic-git-subbuild/sonic-git-populate-prefix/src/sonic-git-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
