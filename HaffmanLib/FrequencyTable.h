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

  template<typename T/*, class InT = typename std::enable_if_t<std::is_same<byte,std::decay_t<decltype(std::declval<T>().operator*)>>::value>*/>
  void takeFrequency(T from, T to);

  unsigned int getFrequencyOf(char symbol) const;
  HaffmanTree getTree();
  void reset();

private:
  std::array<unsigned int, 256> _rawFreqItemBuf;
};

template<typename T/*, class InT = typename std::enable_if_t<std::is_same<byte, std::decay_t<decltype(std::declval<T>().operator*)>>::value>*/>
void FrequencyTable::takeFrequency(T from, T to) {
  for (auto it = from; it != to; ++it)
    ++_rawFreqItemBuf[*it];
}

}
#endif //HAFFMAN_FREQUENCYTABLE_H
