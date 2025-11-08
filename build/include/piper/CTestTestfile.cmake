# CMake generated Testfile for 
# Source directory: D:/Programming Projects/WhisperAIAgent/include/piper
# Build directory: D:/Programming Projects/WhisperAIAgent/build/include/piper
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(test_piper "D:/Programming Projects/WhisperAIAgent/build/include/piper/Debug/test_piper.exe" "D:/Programming Projects/WhisperAIAgent/etc/test_voice.onnx" "D:/Programming Projects/WhisperAIAgent/build/include/piper/pi/share/espeak-ng-data" "D:/Programming Projects/WhisperAIAgent/build/include/piper/test.wav")
  set_tests_properties(test_piper PROPERTIES  _BACKTRACE_TRIPLES "D:/Programming Projects/WhisperAIAgent/include/piper/CMakeLists.txt;110;add_test;D:/Programming Projects/WhisperAIAgent/include/piper/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(test_piper "D:/Programming Projects/WhisperAIAgent/build/include/piper/Release/test_piper.exe" "D:/Programming Projects/WhisperAIAgent/etc/test_voice.onnx" "D:/Programming Projects/WhisperAIAgent/build/include/piper/pi/share/espeak-ng-data" "D:/Programming Projects/WhisperAIAgent/build/include/piper/test.wav")
  set_tests_properties(test_piper PROPERTIES  _BACKTRACE_TRIPLES "D:/Programming Projects/WhisperAIAgent/include/piper/CMakeLists.txt;110;add_test;D:/Programming Projects/WhisperAIAgent/include/piper/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test(test_piper "D:/Programming Projects/WhisperAIAgent/build/include/piper/MinSizeRel/test_piper.exe" "D:/Programming Projects/WhisperAIAgent/etc/test_voice.onnx" "D:/Programming Projects/WhisperAIAgent/build/include/piper/pi/share/espeak-ng-data" "D:/Programming Projects/WhisperAIAgent/build/include/piper/test.wav")
  set_tests_properties(test_piper PROPERTIES  _BACKTRACE_TRIPLES "D:/Programming Projects/WhisperAIAgent/include/piper/CMakeLists.txt;110;add_test;D:/Programming Projects/WhisperAIAgent/include/piper/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test(test_piper "D:/Programming Projects/WhisperAIAgent/build/include/piper/RelWithDebInfo/test_piper.exe" "D:/Programming Projects/WhisperAIAgent/etc/test_voice.onnx" "D:/Programming Projects/WhisperAIAgent/build/include/piper/pi/share/espeak-ng-data" "D:/Programming Projects/WhisperAIAgent/build/include/piper/test.wav")
  set_tests_properties(test_piper PROPERTIES  _BACKTRACE_TRIPLES "D:/Programming Projects/WhisperAIAgent/include/piper/CMakeLists.txt;110;add_test;D:/Programming Projects/WhisperAIAgent/include/piper/CMakeLists.txt;0;")
else()
  add_test(test_piper NOT_AVAILABLE)
endif()
