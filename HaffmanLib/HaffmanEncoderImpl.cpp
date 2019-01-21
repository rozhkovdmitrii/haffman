#include "HaffmanEncoderImpl.h"

namespace HaffmanImpl
{

bool HaffmanEncoderImpl::encode(const VecFreqItem & vecFreqItem, VecByte & buffer) {
  ushort size = (ushort)vecFreqItem.size();
  write(size, buffer);
  for (const FreqItem & freqItem : vecFreqItem)
    encode(freqItem, buffer);
  return true;
}

bool HaffmanEncoderImpl::encode(const FreqItem & freqItem, VecByte & buffer) {
  write(freqItem._sym, buffer);
  write(freqItem._freq, buffer);
  return true;
}

bool HaffmanEncoderImpl::encode(WriteTreeCodeState & state, const TreeCode & treeCode, VecByte & buffer) {
  unsigned short toBeWrote;
  if (state.putAndCheckPossibleToWrite(treeCode, toBeWrote))
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
}