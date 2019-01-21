add_executable(haffmanTest main.cpp EncoderTests.cpp DecoderTests.cpp)

target_include_directories(haffmanTest  PRIVATE
  ${GOOGLE_TEST_DIR}/googletest/include
  ${GOOGLE_TEST_DIR}/googlemock/include)

target_link_libraries(haffmanTest gtest haffmanLib)

