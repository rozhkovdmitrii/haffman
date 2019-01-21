
add_library(haffmanLib
  FrequencyTable.cpp
  HaffmanEncoderImpl.cpp
  HaffmanEncoderImpl.h
  HaffmanDecoderImpl.cpp
  HaffmanDecoderImpl.h
  CommonTypes.h
  Log.h
  Log.cpp

  HaffmanTree/HaffmanTree.cpp
  HaffmanTree/HaffmanTree.h
  HaffmanTree/TreeCode.h
  HaffmanTree/TreeCode.cpp
  HaffmanTree/TreeNode.cpp
  HaffmanTree/TreeNode.h)

target_include_directories(haffmanLib PRIVATE .)

include_directories(. HaffmanTree)
