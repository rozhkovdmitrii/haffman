#include "HaffmanDecoder.h"

namespace Haffman
{

void HaffmanDecoder::addData(const VecByte & data) {
  std::copy(data.begin(), data.end(), std::back_inserter(_data));
  _dataPtr = _data.empty() ? nullptr : &_data[0];
  _dataLength = data.size();
}

Haffman::HaffmanDecoder::HaffmanDecoder() : _dataPtr(nullptr), _dataLength(0) {
}

bool HaffmanDecoder::addDataAndTryToDecode(const VecByte & data) {

  return true;
}

bool HaffmanDecoder::decodeFrequencyTable(FrequencyTable & freqTable) {
  byte size;
  if (_data.empty())
  {
    std::cerr << "ERROR: can not decode: _data ptr isn't set" << std::endl;
    return false;
  }

  if (!read(size))
  {
    std::cerr << "ERROR: can not read size of Haffman frequency table" << std::endl;
    return false;
  }

  for (int i = 0; i < size; i++)
  {
    FreqItem freqItem;
    if (!decodeFrequencyItem(freqItem))
      return false;
    freqTable.setFrequencyOf(freqItem._sym, freqItem._freq);
  }

  return true;
}

bool HaffmanDecoder::decodeFrequencyItem(FreqItem & freqItem) {
  if (!read(freqItem._sym))
  {
    std::cerr << "ERROR: can not read FreqItem._sym" << std::endl;
    return false;
  }
  if (!read(freqItem._freq))
  {
    std::cerr << "ERROR: can not read FreqItem._freq" << std::endl;
    return false;
  }
  return true;
}


}
