#include "HaffmanDecoderImpl.h"

namespace Haffman
{



Haffman::HaffmanDecoderImpl::HaffmanDecoderImpl() : _dataIndex(0), _dataLength(0), _state(State::BlocksCountReading) {
}


bool HaffmanDecoderImpl::processState(VecByte & buffer) {
  auto dataIndex = _dataIndex;
  auto dataLength = _dataLength;
  try {
    switch (_state) {
    case State::BlocksCountReading:
      return processStateBlocksCountReading();
    case State::HeadReading:
      return processStateHeadReading();
    case State::PayloadReading:
      return processStatePayloadReading(buffer);
    case State::Finished:
    case State::Error:
      break;
    }
  } catch (const char * message)
  {
    _dataIndex = dataIndex;
    _dataLength = dataLength;
    _state = State::Error;
  }
  return true;
}

bool HaffmanDecoderImpl::decodeHead() {
  auto dataIndex = _dataIndex;
  auto dataLength = _dataLength;
  if (!decodeFrequencyTableImpl())
  {
    _dataIndex = dataIndex;
    _dataLength = dataLength;
    return false;
  }
  return true;
}

bool HaffmanDecoderImpl::processStateBlocksCountReading() {
  if (!decodeBlocksCount())
    return false;
  _state = State::HeadReading;
  return true;
}

bool HaffmanDecoderImpl::decodeBlocksCount() {
  return !read(_blocksCount) ? ErrLog() << "Blocks count reading failed" : true;
}

bool HaffmanDecoderImpl::decodeFrequencyTableImpl() {
  byte size;
  if (_data.empty())
    return ErrLog() << "can not FileDecoder: _data ptr isn't set";

  if (!read(size))
    return ErrLog() << "can not read size of Haffman frequency table";

  _freqTable.reset();
  for (int i = 0; i < size; i++)
  {
    FreqItem freqItem;
    if (!decodeFrequencyItem(freqItem))
      return false;
    _freqTable.setFrequencyOf(freqItem._sym, freqItem._freq);
  }
  return true;
}

bool HaffmanDecoderImpl::decodeFrequencyItem(FreqItem & freqItem) {
  if (!read(freqItem._sym))
    return ErrLog() << "can not read FreqItem._sym";
  if (!read(freqItem._freq))
    return ErrLog() << "can not read FreqItem._freq";
  return true;
}

const FrequencyTable & HaffmanDecoderImpl::getFrequencyTable() const {
  return _freqTable;
}

bool HaffmanDecoderImpl::decodePayload(VecByte & buffer) {
  auto dataIndex = _dataIndex;
  auto dataLength = _dataLength;
  auto bufLength = buffer.size();

  if (!decodePayloadImpl(buffer)) {
    _dataIndex = dataIndex;
    _dataLength = dataLength;
    buffer.resize(bufLength);
    return false;
  }
  return true;
}

bool HaffmanDecoderImpl::decodePayloadImpl(VecByte & buffer) {

  HaffmanTree haffmanTree = getFrequencyTable().getHaffmanTree();

  uint encCount;
  if (!read(encCount))
    return ErrLog() << "read encrypted values count failed";
  DecTreeCodeState decodeState(encCount, haffmanTree.getTop());

  byte encByte = 0;
  uint decrypedCount = 0;
  while (decrypedCount < encCount) {
    if (!read(encByte))
        return ErrLog() << "read next payload byte failed";
    for (int i = 0; i < 8 && decrypedCount < encCount; ++i) {
      byte decByte;
      if (decodeState.addBitAndTryToDecode(encByte & 1 << 7 - i, decByte)) {
        ++decrypedCount;
        buffer.push_back(decByte);
      }
    }
  }
  return true;
}

bool HaffmanDecoderImpl::processStateHeadReading() {
  if (!decodeHead())
    return false;
  _state = State::PayloadReading;
  return true;
}

bool HaffmanDecoderImpl::processStatePayloadReading(VecByte & buffer) {
  if (!decodePayload(buffer))
    return false;
  --_blocksCount;
  _state = _blocksCount == 0 ? State::Finished : State::HeadReading;
  return true;
}

bool DecTreeCodeState::addBitAndTryToDecode(bool bit, byte & sym) {
  JoinNode * pos = dynamic_cast<JoinNode *>(_pos);
  if (pos == nullptr)
    return ErrLog() << "DecTreeCodeState unexpected state";

  _pos = bit ? pos->getRight() : pos->getLeft();
  if (_pos->getType() == TreeNode::Type::Join)
    return false;
  LeafNode * leaf = dynamic_cast<LeafNode *>(_pos);
  if (leaf == nullptr)
    return false;
  _pos = _top;
  sym = leaf->getSym();

  return true;
}

DecTreeCodeState::DecTreeCodeState(uint encBytesCount, TreeNode * top) :
  _top(top),
  _pos(top),
   _encBytesCount(encBytesCount)
{};

}
