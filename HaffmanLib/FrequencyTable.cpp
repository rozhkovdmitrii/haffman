#include "FrequencyTable.h"
#include <stack>

namespace Haffman
{


int FrequencyTable::getFrequencyOf(char symbol) const {
  return _rawLeafBuf[symbol].getFreq();
}

void FrequencyTable::reset() {
  _symFreqTable.clear();
  for (auto & leafe : _rawLeafBuf)
  {
    leafe = LeafNode();
  }
}

HaffmanTree FrequencyTable::getTree() {
  std::sort(_symFreqTable.begin(), _symFreqTable.end(),
            [](const LeafNode * left, const LeafNode * right) -> bool { return left->getFreq() < right->getFreq(); });
  HaffmanTree tree(_symFreqTable);
  return tree;
}


}