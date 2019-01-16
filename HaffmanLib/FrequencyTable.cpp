#include "FrequencyTable.h"
#include <stack>

namespace Haffman
{

unsigned int FrequencyTable::getFrequencyOf(char symbol) const {
  return _rawFreqItemBuf[symbol];
}

FrequencyTable::FrequencyTable() {
  reset();
}

HaffmanTree FrequencyTable::getTree() {
  VecFreqItem vecFreqItem;
  for (int i = 0; i < _rawFreqItemBuf.size(); ++i)
    if (_rawFreqItemBuf[i] != 0) vecFreqItem.emplace_back((byte)i, _rawFreqItemBuf[i]);
  HaffmanTree tree(vecFreqItem);
  return tree;
}

void FrequencyTable::reset() {
  std::fill_n(_rawFreqItemBuf.begin(), 256, 0);
}

}