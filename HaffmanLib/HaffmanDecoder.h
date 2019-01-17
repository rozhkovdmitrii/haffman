#ifndef HAFFMAN_HAFFMANDECODER_H
#define HAFFMAN_HAFFMANDECODER_H

#include "HaffmanTree.h"
#include "FrequencyTable.h"

namespace Haffman
{

class HaffmanDecoder
{
public:
  HaffmanDecoder();

  void addData(const VecByte & data);
  bool addDataAndTryToDecode(const VecByte & data);

  bool decodeFrequencyTable(FrequencyTable & freqTable);
  bool decodeFrequencyItem(FreqItem & freqItem);

  template <typename T>
  bool read(T & value);

private:
  VecByte _data;
  byte * _dataPtr;
  uint _dataLength;
};

template<typename T>
bool HaffmanDecoder::read(T & value) {
  if (_dataLength < sizeof(T))
  {
    std::cerr << "ERROR: Data length = " << _dataLength  << " is too small. Must be at least: " << sizeof(T);
    return false;
  }
  value = *reinterpret_cast<T *>(_dataPtr);
  _dataPtr += sizeof(T);
  _dataLength -= sizeof(T);
  return true;
}


}

#endif //HAFFMAN_HAFFMANDECODER_H
