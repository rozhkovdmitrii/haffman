#ifndef HAFFMAN_HAFFMANDECODER_H
#define HAFFMAN_HAFFMANDECODER_H

#include "HaffmanTree.h"
#include "FrequencyTable.h"
#include "Log.h"

namespace HaffmanImpl
{

class DecTreeCodeState {
public:
  DecTreeCodeState(uint encBytesCount, TreeNode * top);
  bool addBitAndTryToDecode(bool bit, byte & sym);

private:
  TreeNode * _top;
  TreeNode * _pos;
  const uint _encBytesCount;
  byte _padding;
};


class HaffmanDecoderImpl {
public:
  HaffmanDecoderImpl();

  template <typename T>
  void addData(T begin, T end);
  template <typename T>
  bool addDataAndTryToDecode(T begin, T end, VecByte & buffer);


  bool processState(VecByte & buffer);
  bool decodeHead();
  bool decodePayload(VecByte & buffer);

  template <typename T>
  bool read(T & value);

  const FrequencyTable & getFrequencyTable() const;

private:
  enum class State {
    MagicNumReading,
    BlocksCountReading,
    HeadReading,
    PayloadReading,
    Finished,
    Error
  };

  enum {
    MaxUselessDataSize = 1024
  };

  bool processStateBlocksCountReading();
  bool processStateHeadReading();
  bool processStatePayloadReading(VecByte & buffer);

  bool decodeBlocksCount();
  bool decodeFrequencyTableImpl();
  bool decodeFrequencyItem(FreqItem & freqItem);
  bool decodePayloadImpl(VecByte & buffer);

  void clearUselessData();

  FrequencyTable _freqTable;
  VecByte _data;
  uint _dataIndex;
  uint _dataLength;
  State _state;
  uint _blocksCount;
};

template<typename T>
bool HaffmanDecoderImpl::read(T & value) {
  if (_dataLength < sizeof(T)) {
    return LOG(DBGERR) << "Data length = " << _dataLength << " is too small. Must be at least: " << sizeof(T);
  }
  value = *reinterpret_cast<T *>(&_data[_dataIndex]);
  _dataIndex += sizeof(T);
  _dataLength -= sizeof(T);
  return true;
}


template <typename T>
bool HaffmanDecoderImpl::addDataAndTryToDecode(T begin, T end, VecByte & buffer) {
  addData(begin, end);
  while (_state != State::Finished && _state != State::Error) {
    if (!processState(buffer))
      break;
  }

  if (_dataIndex > MaxUselessDataSize)
    clearUselessData();


  return _state == State::Finished;
}

template <typename T>
void HaffmanDecoderImpl::addData(T begin,T end) {
  auto size = std::distance(begin, end);
  _data.reserve(_data.size() + size);
  std::copy(begin, end, std::back_inserter(_data));
  _dataLength = _data.size() - _dataIndex;

}


}

#endif //HAFFMAN_HAFFMANDECODER_H
