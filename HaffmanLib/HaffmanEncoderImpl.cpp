#include "HaffmanEncoderImpl.h"

namespace HaffmanImpl
{

bool HaffmanEncoderImpl::encode(const VecFreqItem & vecFreqItem, VecByte & buffer) {
  byte size = vecFreqItem.size();
  write(size, buffer);
  for (const FreqItem & freqItem : vecFreqItem)
    encode(freqItem, buffer);
  return true;
}

bool HaffmanEncoderImpl::encode(const FreqItem & freqItem, VecByte & buffer) {
  write(freqItem._sym, buffer);
  write(freqItem._freq, buffer);
}

template<typename T>
void HaffmanEncoderImpl::write(T value, VecByte & buffer) {
  size_t oldSize = buffer.size();
  buffer.resize(oldSize + sizeof(T));
  *reinterpret_cast<T *>(&buffer[oldSize]) = value;
  _wroteSize += sizeof(T);
}

bool HaffmanEncoderImpl::encode(WriteTreeCodeState & state, const TreeCode & treeCode, VecByte & buffer) {
  byte toBeWrote;
  if (state.emplace(treeCode, toBeWrote))
    write(toBeWrote, buffer);
  return true;
}

bool HaffmanEncoderImpl::encodeHeader(const FrequencyTable & freqTable, VecByte & buffer) {
  return encode(freqTable.getFreqPack(), buffer);
}

const HaffmanTree & HaffmanEncoderImpl::getHaffmanTree() const {
  return _haffmanTree;
}

bool WriteTreeCodeState::emplace(const TreeCode & treeCode, byte & toBeWrote) {
  int rest = DigitCount - _bufferedCount;
  if (rest > treeCode._size) {
    byte valToBuf = treeCode._base << rest - treeCode._size;
    _buffer |= valToBuf;
    _bufferedCount += treeCode._size;
    return false;
  }
  _buffer |= treeCode._base >> treeCode._size - rest;
  toBeWrote = _buffer;
  _buffer = treeCode._base << DigitCount - treeCode._size + rest;
  _bufferedCount = treeCode._size - rest;
  return true;
}

}