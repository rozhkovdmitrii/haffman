#ifndef HAFFMAN_FREQUENCYTABLE_H
#define HAFFMAN_FREQUENCYTABLE_H

#include <array>
#include <algorithm>
#include <vector>
#include <iostream>

#include "HaffmanTree.h"

namespace HaffmanImpl {

class FrequencyTable {
public:
  FrequencyTable();
  ~FrequencyTable() = default;

  template<typename T>
  void takeFrequency(T from, T to);

  VecFreqItem getFreqPack() const;
  uint getFrequencyOf(byte symb) const;
  void setFrequencyOf(byte symb, uint freq);
  HaffmanTree getHaffmanTree() const;
  void reset();

private:
  ArrUInt256 _rawFreqBuf;
};

template<typename T>
void FrequencyTable::takeFrequency(T from, T to) {
  for (auto it = from; it != to; ++it)
    ++_rawFreqBuf[*it];
}

}
#endif //HAFFMAN_FREQUENCYTABLE_H
