//----------------------------------------------------------------------------------------------------------------------
#ifndef HAFFMAN_HAFFMANENCODER_H
#define HAFFMAN_HAFFMANENCODER_H
//----------------------------------------------------------------------------------------------------------------------
#include "Log.h"
#include "FrequencyTable.h"
//----------------------------------------------------------------------------------------------------------------------
namespace HaffmanImpl
{
//----------------------------------------------------------------------------------------------------------------------
class HaffmanEncoderImpl
{
public:
  HaffmanEncoderImpl() = default;

  template <typename T>
  void prepareToEncode(T begin, T end);
  const FrequencyTable & getFrequencyTable() const;
  const HaffmanTree & getHaffmanTree() const;

  template <typename T>
  bool encodeBlock(T begin, T end, VecByte & buffer);
  bool encodeHeader(const FrequencyTable & freqTable, VecByte & buffer);
  template <typename T>
  bool encodePayload(T begin, T end, VecByte & buffer);

  bool encode(const FreqItem &, VecByte & buffer);

  template <typename T>
  void write( const T & value, VecByte & buffer);

private:
  bool encode(WriteTreeCodeState & state, const TreeCode & treeCode, VecByte & buffer);
  bool encode(const VecFreqItem &, VecByte & buffer);

  FrequencyTable _freqTable;
  uint _wroteSize = 0;
};
//----------------------------------------------------------------------------------------------------------------------
template<typename T>
void HaffmanEncoderImpl::write(const T & value, VecByte & buffer) {
  size_t oldSize = buffer.size();
  buffer.resize(oldSize + sizeof(T));
  *reinterpret_cast<T *>(&buffer[oldSize]) = value;
  _wroteSize += sizeof(T);
}
//----------------------------------------------------------------------------------------------------------------------
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
  _freqTable.buildTree();
}
//----------------------------------------------------------------------------------------------------------------------
template <typename T>
bool HaffmanEncoderImpl::encodePayload(T begin, T end, VecByte & buffer) {
  WriteTreeCodeState encPayloadState;
  uint baseBuffSize = buffer.size();
  uint blockSize = (uint)std::distance(begin, end);
  write(blockSize, buffer); //write size

  for (auto i = begin; i != end; ++i) {
    const TreeCode & code = _freqTable.getHaffmanTree().getCode((byte)*i);
    if (code._size == 0)
      return LOG(DBGERR) << "encoding not processed in HaffmanTree value: '" << *i << "'";
    encode(encPayloadState, code, buffer);
  }

  if (!encPayloadState.isEmpty())
    write(encPayloadState._buffer, buffer);

  uint wroteSize =  buffer.size() - baseBuffSize;
  if (wroteSize < 4)
    return LOG(DBGERR) << "Encode TreeCode vector: buffer should contain at list 5 bytes";
  return true;
}
//----------------------------------------------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------------------------------------------
#endif //HAFFMAN_HAFFMANENCODER_H
//----------------------------------------------------------------------------------------------------------------------
