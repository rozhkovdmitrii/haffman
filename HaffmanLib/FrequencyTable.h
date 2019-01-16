#ifndef HAFFMAN_FREQUENCYTABLE_H
#define HAFFMAN_FREQUENCYTABLE_H

#include <array>
#include <algorithm>
#include <vector>
#include <iostream>

#include "HaffmanTree.h"

namespace Haffman {

class FrequencyTable {
public:
  FrequencyTable();
  ~FrequencyTable() = default;

  template<typename T>
  void takeFrequency(T from, T to);

  int getFrequencyOf(char symbol) const;
  HaffmanTree getTree();
  void reset();

private:
  std::array<FreqItem, 256> _rawFreqItemBuf;
  VecFreqItemPtr _freqItemsGot;
};

template<typename T>
void FrequencyTable::takeFrequency(T from, T to) {
  for (auto it = from; it != to; ++it) {
    FreqItem & freqItem = _rawFreqItemBuf[*it];
    if (freqItem._freq == 0)
      _freqItemsGot.push_back(&freqItem);
    ++freqItem._freq;
  }
}

}
#endif //HAFFMAN_FREQUENCYTABLE_H
