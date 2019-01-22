//----------------------------------------------------------------------------------------------------------------------
#ifndef HAFFMAN_HAFFMANDECODER_H
#define HAFFMAN_HAFFMANDECODER_H
//----------------------------------------------------------------------------------------------------------------------
#include "FrequencyTable.h"
#include "Log.h"
//----------------------------------------------------------------------------------------------------------------------
namespace HaffmanImpl
{
//----------------------------------------------------------------------------------------------------------------------
class HaffmanDecoderImpl {
public:
  HaffmanDecoderImpl();

  template <typename T>
  void addData(T begin, T end);
  template <typename T>
  bool addDataAndTryToDecode(T begin, T end, VecByte & buffer);

  bool decodeHead();
  bool decodePayload(VecByte & buffer);

  const FrequencyTable & getFrequencyTable() const;
  const bool isFinished() const;

private:
  enum class State {
    BlocksCountReading,
    HeadReading,
    PayloadReading,
    Finished
  };

  enum {
    MaxUselessDataSize = 1024
  };

  template <typename T>
  bool read(T & value);

  bool processState(VecByte & buffer);

  bool processStateBlocksCountReading();
  bool decodeBlocksCount();

  bool processStateHeadReading();
  bool decodeFrequencyItem(FreqItem & freqItem);
  bool decodeFrequencyTableImpl();

  bool processStatePayloadReading(VecByte & buffer);

  void clearUselessData();

  FrequencyTable _freqTable;
  VecByte _data;
  uint _dataIndex;
  uint _dataLength;
  State _state;
  uint _blocksCount;
};
//----------------------------------------------------------------------------------------------------------------------
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
//----------------------------------------------------------------------------------------------------------------------
template <typename T>
bool HaffmanDecoderImpl::addDataAndTryToDecode(T begin, T end, VecByte & buffer) {
  addData(begin, end);
  while (_state != State::Finished) {
    auto dataIndex = _dataIndex;
    auto dataLength = _dataLength;
    auto bufferSize = buffer.size();
    if (!processState(buffer))
    {
      _dataIndex = dataIndex;
      _dataLength = _dataLength;
      buffer.resize(bufferSize);
      break;
    }
  }

  if (_dataIndex > MaxUselessDataSize)
    clearUselessData();

  return _state == State::Finished;
}
//----------------------------------------------------------------------------------------------------------------------
template <typename T>
void HaffmanDecoderImpl::addData(T begin,T end) {
  auto size = std::distance(begin, end);
  _data.reserve(_data.size() + size);
  std::copy(begin, end, std::back_inserter(_data));
  _dataLength = _data.size() - _dataIndex;
}
//----------------------------------------------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------------------------------------------
#endif //HAFFMAN_HAFFMANDECODER_H
