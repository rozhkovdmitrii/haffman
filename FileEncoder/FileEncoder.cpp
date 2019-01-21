//----------------------------------------------------------------------------------------------------------------------
#include <iostream>
#include <cmath>
#include <cstring>
//----------------------------------------------------------------------------------------------------------------------
#include "CommonTypes.h"
#include "Log.h"
#include "FileEncoder.h"
//----------------------------------------------------------------------------------------------------------------------
namespace Haffman
{
//----------------------------------------------------------------------------------------------------------------------
FileEncoder::FileEncoder(const std::string & ifPath, const std::string & ofPath) :
  _ifPath(ifPath),
  _ofPath(ofPath),
  _ifstream(ifPath,  std::ios::binary | std::ios::ate),
  _ofstream(ofPath, std::ios::binary) {
}
//----------------------------------------------------------------------------------------------------------------------
bool FileEncoder::encodeInputFile() {
  return isReadyToEncode() && encodeMagicNum() && encodeBlocksCount() && encodeBlocks();
}
//----------------------------------------------------------------------------------------------------------------------
const std::string & FileEncoder::getIfPath() const {
  return _ifPath;
}
//----------------------------------------------------------------------------------------------------------------------
bool FileEncoder::isReadyToEncode() const {
  if (!_ifstream.good())
    return LOG(APPERR) << "Input file is not good: " << _ifPath;

  if (!_ofstream.good())
    return LOG(APPERR) << "Out file is not good: " << _ofPath;
  return true;
}
//----------------------------------------------------------------------------------------------------------------------
bool FileEncoder::encodeMagicNum() {
  if (!_ofstream.write(reinterpret_cast<const char *>(&HaffmanImpl::MagicNum), sizeof(unsigned long long)))
    return LOG(DBGERR) << strerror(errno);
  _ofstream.flush();
  return true;
}
//----------------------------------------------------------------------------------------------------------------------
bool FileEncoder::encodeBlocksCount() {
  size_t inFileSize;
  if (!getInStreamSize(inFileSize))
    return false;
  _blocksCount = std::ceil((double)inFileSize / BlockSize);
  if (!_ofstream.write(reinterpret_cast<const char *>(&_blocksCount), sizeof(_blocksCount)))
    return LOG(DBGERR) << strerror(errno);
  _ofstream.flush();
  LOG(DBGINF) << "Blocks count: " << _blocksCount;
  return true;
}
//----------------------------------------------------------------------------------------------------------------------
bool FileEncoder::getInStreamSize(size_t & size) {

  std::streampos endPos = _ifstream.tellg();
  if (endPos == static_cast<const std::streamoff>(-1))
    return LOG(APPERR) << "Can't get end pos for: " << _ifPath;
  if (!_ifstream.seekg(0, std::ios::beg))
    return LOG(APPERR) << "Can't set file beg pos for: " << _ifPath;
  std::streampos begPos = _ifstream.tellg();
  if (begPos == static_cast<const std::streamoff>(-1))
    return LOG(APPERR) << "Can't get file pos for: " << _ifPath;

  size = endPos - begPos;
  return true;
}
//----------------------------------------------------------------------------------------------------------------------
bool FileEncoder::encodeBlocks() {
  int num = 0;
  while (auto len = _ifstream.readsome((char *)_buffer.begin(), BlockSize)) {
    encodeBlock((char *)_buffer.data(), (char *)_buffer.data() + len);
    num++;
    LOG(APPINF) << "Block:" << num << " encoded";
  }
  return true;
}
//----------------------------------------------------------------------------------------------------------------------
bool FileEncoder::encodeBlock(char * from, char * to) {
  VecByte encBuf;
  if (!_haffmanEncoder.encodeBlock(from, to, encBuf))
    return LOG(DBGERR) << "Encoding block failed";
  if (!_ofstream.write(reinterpret_cast<const char *>(encBuf.data()), encBuf.size()))
    return LOG(DBGERR) << "Writing encoded block failed";
  _ofstream.flush();
  return true;
}
//----------------------------------------------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------------------------------------------
