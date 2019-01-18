#ifndef HAFFMAN_HAFFMANENCODER_H
#define HAFFMAN_HAFFMANENCODER_H

#include "Log.h"
#include "FrequencyTable.h"

namespace HaffmanImpl
{

struct WriteTreeCodeState {
  enum {
    DigitCount = sizeof(byte) * 8
  };

  byte _buffer = 0;
  byte _bufferedCount = 0;
  bool isEmpty() const { return _bufferedCount == 0; }
  bool emplace(const TreeCode & treeCode, byte & toBeWrote);

  void reset() { _buffer = 0; _bufferedCount = 0; }
};

class HaffmanEncoderImpl
{
public:
  HaffmanEncoderImpl() = default;

  template <typename T>
  void prepareToEncode(T begin, T end);
  const HaffmanTree & getHaffmanTree() const;

  template <typename T>
  bool encodeBlock(T begin, T end, VecByte & buffer);
  bool encodeHeader(const FrequencyTable & freqTable, VecByte & buffer);
  template <typename T>

  bool encodePayload(T begin, T end, VecByte & buffer);
  bool encode(const VecFreqItem &, VecByte & buffer);
  bool encode(const FreqItem &, VecByte & buffer);
  bool encode(WriteTreeCodeState & state, const TreeCode & treeCode, VecByte & buffer);

  template <typename T>
  void write(T value, VecByte & buffer);

private:
  FrequencyTable _freqTable;
  HaffmanTree _haffmanTree;
  WriteTreeCodeState _writeCodeState;
  uint _wroteSize = 0;
};

template<typename T>
bool HaffmanEncoderImpl::encodeBlock(T begin, T end, VecByte & buffer) {
  prepareToEncode(begin, end);

  if (!encodeHeader(_freqTable, buffer))
    return false;

  if (!encodePayload(begin, end, buffer))
    return false;

  return true;
}

template <typename T>
void HaffmanEncoderImpl::prepareToEncode(T begin, T end) {
  _freqTable.reset();
  _freqTable.takeFrequency(begin, end);
  _haffmanTree = _freqTable.getHaffmanTree();
}

template <typename T>
bool HaffmanEncoderImpl::encodePayload(T begin, T end, VecByte & buffer) {
  WriteTreeCodeState encPayloadState;
  uint baseBuffSize = buffer.size();
  write((uint)std::distance(begin, end), buffer); //write size
  for (auto i = begin; i != end; ++i) {
    const TreeCode & code = _haffmanTree.getCode(*i);
    if (code._size == 0)
      return LOG(DBGERR) << "encoding not processed in HaffmanTree value: '" << *i << "'";
    encode(encPayloadState, code, buffer);
  }

  if (!encPayloadState.isEmpty())
    write(encPayloadState._buffer, buffer);

  uint wroteSize =  buffer.size() - baseBuffSize;
  if (wroteSize < 4)
    return LOG(DBGERR) << "ERROR: Encode TreeCode vector: buffer should contain at list 5 bytes";
  return true;
}

}

#endif //HAFFMAN_HAFFMANENCODER_H
