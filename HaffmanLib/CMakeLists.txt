
add_library(haffmanLib
  FrequencyTable.cpp
  HaffmanTree.cpp
  HaffmanTree.h
  HaffmanEncoderImpl.cpp
  HaffmanEncoderImpl.h
  HaffmanDecoderImpl.cpp
  HaffmanDecoderImpl.h
  CommonTypes.h
  ErrLog.h
  ErrLog.cpp
)

target_include_directories(haffmanLib PRIVATE .)

include_directories(.)

add_subdirectory(test)