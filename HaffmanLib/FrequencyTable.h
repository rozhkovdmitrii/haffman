//----------------------------------------------------------------------------------------------------------------------
#ifndef HAFFMAN_FREQUENCYTABLE_H
#define HAFFMAN_FREQUENCYTABLE_H
//----------------------------------------------------------------------------------------------------------------------
#include "HaffmanTree.h"
//----------------------------------------------------------------------------------------------------------------------
namespace HaffmanImpl
{
//----------------------------------------------------------------------------------------------------------------------
class FrequencyTable {
public:
  FrequencyTable();
  ~FrequencyTable() = default;

  template<typename T>
  void takeFrequency(T from, T to);

  VecFreqItem getFreqPack() const;
  uint getFrequencyOf(byte symb) const;
  void setFrequencyOf(byte symb, uint freq);

  const HaffmanTree & getHaffmanTree() const;
  void reset();
  void buildTree();

private:
  HaffmanTree _haffmanTree;
  ArrUInt256 _rawFreqBuf;
};
//----------------------------------------------------------------------------------------------------------------------
template<typename T>
void FrequencyTable::takeFrequency(T from, T to) {
  for (auto it = from; it != to; ++it)
    ++_rawFreqBuf[(byte)*it];
}
//----------------------------------------------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------------------------------------------
#endif //HAFFMAN_FREQUENCYTABLE_H
//----------------------------------------------------------------------------------------------------------------------
