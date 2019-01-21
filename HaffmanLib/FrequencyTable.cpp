#include "FrequencyTable.h"
#include <stack>

namespace HaffmanImpl
{

uint FrequencyTable::getFrequencyOf(byte symb) const {
  return _rawFreqBuf[symb];
}

FrequencyTable::FrequencyTable() {
  reset();
}

void FrequencyTable::reset() {
  _haffmanTree.reset();
  std::fill(_rawFreqBuf.begin(), _rawFreqBuf.end(), 0);
}

const HaffmanTree & FrequencyTable::getHaffmanTree() const {
  return _haffmanTree;
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

void FrequencyTable::buildTree() {
  VecFreqItem vecFreqItem;
  for (uint i = 0; i < _rawFreqBuf.size(); ++i) {
    if (_rawFreqBuf[i] != 0)
      vecFreqItem.emplace_back((byte) i, _rawFreqBuf[i]);
  }
  _haffmanTree.resetFrom(vecFreqItem);
}

}