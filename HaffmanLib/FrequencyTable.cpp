//----------------------------------------------------------------------------------------------------------------------
#include "FrequencyTable.h"
//----------------------------------------------------------------------------------------------------------------------
namespace HaffmanImpl
{
//----------------------------------------------------------------------------------------------------------------------
FrequencyTable::FrequencyTable() {
  reset();
}
//----------------------------------------------------------------------------------------------------------------------
VecFreqItem FrequencyTable::getFreqPack() const {
  VecFreqItem vecFreqItem;
  for (int i = 0; i < _rawFreqBuf.size(); ++i) {
    uint freq = _rawFreqBuf[(byte) i];
    if (freq == 0)
      continue;
    vecFreqItem.emplace_back((byte)i, freq);
  }
  return vecFreqItem;
}
//----------------------------------------------------------------------------------------------------------------------
uint FrequencyTable::getFrequencyOf(byte symb) const {
  return _rawFreqBuf[symb];
}
//----------------------------------------------------------------------------------------------------------------------
void FrequencyTable::setFrequencyOf(byte symb, uint freq) {
  _rawFreqBuf[symb] = freq;
}
//----------------------------------------------------------------------------------------------------------------------
void FrequencyTable::reset() {
  _haffmanTree.reset();
  std::fill(_rawFreqBuf.begin(), _rawFreqBuf.end(), 0);
}
//----------------------------------------------------------------------------------------------------------------------
const HaffmanTree & FrequencyTable::getHaffmanTree() const {
  return _haffmanTree;
}
//----------------------------------------------------------------------------------------------------------------------
void FrequencyTable::buildTree() {
  VecFreqItem vecFreqItem;
  for (uint i = 0; i < _rawFreqBuf.size(); ++i) {
    if (_rawFreqBuf[i] != 0)
      vecFreqItem.emplace_back((byte) i, _rawFreqBuf[i]);
  }
  _haffmanTree.resetFrom(vecFreqItem);
}
//----------------------------------------------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------------------------------------------
