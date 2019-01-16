#include "FrequencyTable.h"
#include <stack>

namespace Haffman
{

int FrequencyTable::getFrequencyOf(char symbol) const {
  return _rawFreqItemBuf[symbol]._freq;
}

void FrequencyTable::reset() {
  _freqItemsGot.clear();
  for (char i = 0; i < _rawFreqItemBuf.size(); ++i)
    _rawFreqItemBuf[i] = FreqItem({i, 0});
}

HaffmanTree FrequencyTable::getTree() {
  HaffmanTree tree(_freqItemsGot);
  return tree;
}

FrequencyTable::FrequencyTable() {
  reset();
}

}