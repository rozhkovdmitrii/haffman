#ifndef HAFFMAN_HAFFMANENCODER_H
#define HAFFMAN_HAFFMANENCODER_H

#include "FrequencyTable.h"

namespace Haffman
{


struct TreeCodeBuff {
  enum {
    DigitCount = sizeof(byte) * 8
  };

  byte _buffer = 0;
  byte _bufferedCount = 0;
  bool isEmpty() const { return _bufferedCount == 0; }
  bool emplace(const TreeCode & treeCode, byte & toBeWrote);

  void reset() { _buffer = 0; _bufferedCount = 0; }

  byte getPaddingSize() const;
};

class HaffmanEncoder
{
public:

  enum {
    ReadBlockSize = 1024 * 1024 * 5
  };

  HaffmanEncoder() = default;

  template <typename T>
  bool encodeBlock(T begin, T end, VecByte & buffer);
  bool encodeHeader(const FrequencyTable & freqTable, VecByte & buffer);
  template <typename T>
  bool encodePayload(T begin, T end, VecByte & buffer);
  bool encode(const VecFreqItem &, VecByte & buffer);

  bool encode(const FreqItem &, VecByte & buffer);
  bool encode(const VecTreeCode &, VecByte & buffer);
  bool encode(const TreeCode & treeCode, VecByte & buffer);

  template <typename T>
  void write(T value, VecByte & buffer);

private:
  FrequencyTable _freqTable;
  HaffmanTree _haffmanTree;
  TreeCodeBuff _writeCodeState;
  uint _wroteSize = 0;

  enum {
    ReadByCount = 3
  };
};

template<typename T>
bool HaffmanEncoder::encodeBlock(T begin, T end, VecByte & buffer) {
  _freqTable.takeFrequency(begin, end);
  _haffmanTree = _freqTable.getHaffmanTree();

  if (!encodeHeader(_freqTable, buffer))
    return false;

  if (!encodePayload(begin, end, buffer))
    return false;

  return true;
}

template <typename T>
bool HaffmanEncoder::encodePayload(T begin, T end, VecByte & buffer) {
  uint baseBuffSize = buffer.size();
  write((uint)0, buffer); //write size
  write((byte)0, buffer); //write padding
  for (auto i = begin; i != end; ++i)
    encode(_haffmanTree.getCode(*i), buffer);

  if (!_writeCodeState.isEmpty())
    write(_writeCodeState._buffer, buffer);

  uint wroteSize =  buffer.size() - baseBuffSize;
  if (wroteSize < 5)
  {
    std::cerr << "ERROR: Encode TreeCode vector: buffer should contain at list 5 bytes" << std::endl;
    return false;
  }
  *reinterpret_cast<uint*>(&buffer[baseBuffSize + 0]) = wroteSize;
  *reinterpret_cast<byte*>(&buffer[baseBuffSize + 4]) = _writeCodeState.getPaddingSize();
  return true;
}
}

#endif //HAFFMAN_HAFFMANENCODER_H
