#include "FrequencyTable.h"
#include <stack>

namespace HaffmanImpl
{

unsigned int FrequencyTable::getFrequencyOf(byte symb) const {
  return _rawFreqBuf[symb];
}

FrequencyTable::FrequencyTable() {
  reset();
}

void FrequencyTable::reset() {
  std::fill(_rawFreqBuf.begin(), _rawFreqBuf.end(), 0);
}

HaffmanTree FrequencyTable::getHaffmanTree() const {
  VecFreqItem vecFreqItem;
  for (int i = 0; i < _rawFreqBuf.size(); ++i)
    if (_rawFreqBuf[i] != 0) vecFreqItem.emplace_back((byte)i, _rawFreqBuf[i]);
  return HaffmanTree(vecFreqItem);
}

VecFreqItem FrequencyTable::getFreqPack() const { //TODO: optimize
  VecFreqItem vecFreqItem;
  for (int i = 0; i < 256; ++i)
  {
    uint freq = _rawFreqBuf[(byte) i];
    if (freq == 0)
      continue;
    vecFreqItem.emplace_back((byte)i, freq);
  }
  return vecFreqItem;
}

void FrequencyTable::setFrequencyOf(byte symb, uint freq) {
  _rawFreqBuf[symb] = freq;
}

}