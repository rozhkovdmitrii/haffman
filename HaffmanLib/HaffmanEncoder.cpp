#include "HaffmanEncoder.h"

namespace Haffman
{

bool HaffmanEncoder::encode(const VecFreqItem & vecFreqItem, VecByte & buffer) {
  byte size = vecFreqItem.size();
  write(size, buffer);
  for (const FreqItem & freqItem : vecFreqItem)
    encode(freqItem, buffer);
  return true;
}

bool HaffmanEncoder::encode(const FreqItem & freqItem, VecByte & buffer) {
  write(freqItem._sym, buffer);
  write(freqItem._freq, buffer);
}

template<typename T>
void HaffmanEncoder::write(T value, VecByte & buffer) {
  size_t oldSize = buffer.size();
  buffer.resize(oldSize + sizeof(T));
  *reinterpret_cast<T *>(&buffer[oldSize]) = value;
  _wroteSize += sizeof(T);
}

bool HaffmanEncoder::encode(const TreeCode & treeCode, VecByte & buffer) {
  byte toBeWrote;
  if (_writeCodeState.emplace(treeCode, toBeWrote))
    write(toBeWrote, buffer);
  return true;
}


bool HaffmanEncoder::encode(const VecTreeCode & vecTreeCode, VecByte & buffer) {

  if (!buffer.empty())
  {
    std::cerr << "ERROR: Encode TreeCode vector: buffer should be empty." << std::endl;
    return false;
  }
  uint baseBuffSize = buffer.size();
  write((uint)0, buffer); //write size
  write((byte)0, buffer); //write padding
  for (const TreeCode & treeCode : vecTreeCode)
    encode(treeCode, buffer);

  if (!_writeCodeState.isEmpty())
    write(_writeCodeState._buffer, buffer);

  if (buffer.size() < 5)
  {
    std::cerr << "ERROR: Encode TreeCode vector: buffer should contain at list 5 bytes" << std::endl;
    return false;
  }
  *reinterpret_cast<uint*>(&buffer[baseBuffSize + 0]) = _wroteSize;
  *reinterpret_cast<byte*>(&buffer[baseBuffSize + 4]) = _writeCodeState.getPaddingSize();
}

bool HaffmanEncoder::encodeHeader(const FrequencyTable & freqTable, VecByte & buffer) {
  return encode(freqTable.getFreqPack(), buffer);
}

bool TreeCodeBuff::emplace(const TreeCode & treeCode, byte & toBeWrote) {
  int rest = DigitCount - _bufferedCount;
  if (rest > treeCode._size) {
    byte valToBuf = treeCode._base << rest - treeCode._size;
    _buffer |= valToBuf;
    _bufferedCount += treeCode._size;
    return false;
  } else {
    _buffer |= treeCode._base >> treeCode._size - rest;
    toBeWrote = _buffer;
    _buffer = treeCode._base << DigitCount - treeCode._size + rest;
    _bufferedCount = treeCode._size - rest;
    return true;
  }
  return false;
}

byte TreeCodeBuff::getPaddingSize() const {
  return _bufferedCount == 0 ? 0 : DigitCount - _bufferedCount;
}

}