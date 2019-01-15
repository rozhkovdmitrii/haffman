#ifndef HAFFMAN_FREQUENCYTABLE_H
#define HAFFMAN_FREQUENCYTABLE_H

#include <array>
#include <algorithm>
#include <vector>
#include <iostream>
#include "HaffmanTree.h"

namespace Haffman
{

class FrequencyTable {
public:
  FrequencyTable() = default;
  ~FrequencyTable() = default;

  template<typename T>
  void takeFrequency(T from, T to);

  int getFrequencyOf(char symbol) const;

  HaffmanTree getTree();
  void reset();

private:
  std::array<LeafNode, 256> _rawLeafBuf;
  VecLeafNodePtr _symFreqTable;
};

template<typename T>
void FrequencyTable::takeFrequency(T from, T to) {
  for (auto it = from; it != to; ++it)
  {
    unsigned char index = *it;
    LeafNode & leafe = _rawLeafBuf[index];
    if (leafe.getFreq() == 0)
    {
      _symFreqTable.push_back(&leafe);
      leafe._sym = *it;
    }
    leafe.setFreq(leafe.getFreq() + 1);
  }
}

}
#endif //HAFFMAN_FREQUENCYTABLE_H
