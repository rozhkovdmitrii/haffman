add_executable(decode main.cpp FileDecoder.h FileDecode.cpp)

target_link_libraries(decode haffmanLib)
