
add_library(haffmanLib
  FrequencyTable.cpp
  HaffmanEncoderImpl.cpp
  HaffmanEncoderImpl.h
  HaffmanDecoderImpl.cpp
  HaffmanDecoderImpl.h
  CommonTypes.h
  Log.h
  Log.cpp

  HaffmanTree.cpp
  HaffmanTree.h
  TreeCode.h
  TreeCode.cpp
  TreeNode.cpp
  TreeNode.h)

target_include_directories(haffmanLib PRIVATE .)

include_directories(. HaffmanTree)
