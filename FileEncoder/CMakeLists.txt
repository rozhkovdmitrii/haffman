add_executable(encode main.cpp FileEncoder.h FileEncoder.cpp ../FileDecoder/FileDecoder.cpp ../FileDecoder/FileDecoder.h)
target_link_libraries(encode haffmanLib)
