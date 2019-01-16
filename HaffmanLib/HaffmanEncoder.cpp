#include "HaffmanEncoder.h"

namespace Haffman
{
template <typename T>
bool HaffmanEncoder::encode(T begin, T end, VecByte & buffer) {
  FrequencyTable frequencyTable;

  for (auto it = begin; it != end;)
  {
    auto blockBegin = it;
    auto blockEnd = std::distance(it, end) > ReadByCount ? it + ReadByCount : end;
    frequencyTable.takeFrequency(blockBegin, blockEnd);
    it = blockEnd;
  }

/*
  for (int i = 0; i < sizeof(byte); ++i)
    encode()
*/


  return false;
}

bool HaffmanEncoder::encode(const VecFreqItem & vecFreqItem, VecByte & buffer) {
  for (const FreqItem & freqItem : vecFreqItem)
    encode(freqItem, buffer);
}

bool HaffmanEncoder::encode(const FreqItem & freqItem, VecByte & buffer) {
  write(freqItem._sym, buffer);
  write(freqItem._freq, buffer);
}

template<typename T>
void HaffmanEncoder::write(T value, VecByte & buffer) const {
  size_t oldSize = buffer.size();
  buffer.resize(oldSize + sizeof(T));
  *reinterpret_cast<T *>(&buffer[oldSize]) = value;
}

}