#include "HaffmanEncoderImpl.h"

namespace HaffmanImpl
{

bool HaffmanEncoderImpl::encode(const VecFreqItem & vecFreqItem, VecByte & buffer) {
  ushort size = vecFreqItem.size();
  write(size, buffer);
  for (const FreqItem & freqItem : vecFreqItem)
    encode(freqItem, buffer);
  return true;
}

bool HaffmanEncoderImpl::encode(const FreqItem & freqItem, VecByte & buffer) {
  write(freqItem._sym, buffer);
  write(freqItem._freq, buffer);
}

bool HaffmanEncoderImpl::encode(WriteTreeCodeState & state, const TreeCode & treeCode, VecByte & buffer) {
  unsigned short toBeWrote;
  if (state.emplace(treeCode, toBeWrote))
    write(toBeWrote, buffer);
  return true;
}

bool HaffmanEncoderImpl::encodeHeader(const FrequencyTable & freqTable, VecByte & buffer) {
  VecFreqItem vecFreqItem = freqTable.getFreqPack();
  return encode(vecFreqItem, buffer);
}

const HaffmanTree & HaffmanEncoderImpl::getHaffmanTree() const {
  return _freqTable.getHaffmanTree();
}

const FrequencyTable & HaffmanEncoderImpl::getFrequencyTable() const {
  return _freqTable;
}

bool WriteTreeCodeState::emplace(const TreeCode & treeCode, unsigned short & toBeWrote) {
  int rest = DigitCount - _bufferedCount;
  if (rest > treeCode._size) {
    unsigned short valToBuf = treeCode._base << rest - treeCode._size;
    _buffer |= valToBuf;
    _bufferedCount += treeCode._size;
    return false;
  }
  toBeWrote = _buffer | treeCode._base >> treeCode._size - rest;
  _buffer = treeCode._base << DigitCount - treeCode._size + rest;
  _bufferedCount = treeCode._size - rest;
  return true;
}

}