#include "FrequencyTable.h"
#include <stack>

namespace Haffman
{

unsigned int FrequencyTable::getFrequencyOf(byte symbol) const {
  return _rawFreqBuf[symbol];
}

FrequencyTable::FrequencyTable() {

  reset();
}

HaffmanTree FrequencyTable::getHaffmanTree() const {
  VecFreqItem vecFreqItem;
  for (int i = 0; i < _rawFreqBuf.size(); ++i)
    if (_rawFreqBuf[i] != 0) vecFreqItem.emplace_back((byte)i, _rawFreqBuf[i]);
  HaffmanTree tree(vecFreqItem);
  return tree;
}

void FrequencyTable::reset() {
  std::fill(_rawFreqBuf.begin(), _rawFreqBuf.end(), 0);
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

}