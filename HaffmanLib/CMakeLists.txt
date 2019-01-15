add_library(haffmanLib FrequencyTable.cpp HaffmanTree.cpp HaffmanTree.h HaffmanEncoder.cpp HaffmanEncoder.h HaffmanDecoder.cpp HaffmanDecoder.h)
target_include_directories(haffmanLib PRIVATE .)

include_directories(.)

add_subdirectory(test)