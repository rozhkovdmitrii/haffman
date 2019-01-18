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
FileEncoder::operator bool() const {
  if (!_ifstream.good())
  {
    std::cerr << "Input file is not good: " << _ifPath;
    return false;
  }

  if (!_ofstream.good())
  {
    std::cerr << "Out file is not good: " << _ofPath;
    return false;
  };
  return true;
}
//----------------------------------------------------------------------------------------------------------------------
bool FileEncoder::encodeMagicNum() {
  static const unsigned long long magicNum = 0xFEDCBA98FEDCBA98;
  if (!_ofstream.write(reinterpret_cast<const char *>(&magicNum), sizeof(unsigned long long)))
    return LOG(DBGERR) << strerror(errno);
  return true;
}
//----------------------------------------------------------------------------------------------------------------------
bool FileEncoder::encodeBlock() {
  auto readedSize = _ifstream.readsome(_buffer.data(), _buffer.size());
  VecByte encBuf;
  if (!_haffmanEncoder.encodeBlock(_buffer.begin(), _buffer.begin() + readedSize, encBuf))
    return LOG(DBGERR) << "Encoding block failed";
  if (!_ofstream.write((const char *)encBuf.data(), encBuf.size()))
    return LOG(DBGERR) << "Writing encoded block failed";
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
  auto blocksCount = _blocksCount;
  while (blocksCount--) {
    if (!encodeBlock())
      return LOG(DBGERR) << "Encoding next block failed: " << blocksCount + 1;
  }
  return true;
}
//----------------------------------------------------------------------------------------------------------------------
const std::string & FileEncoder::getIfPath() const {
  return _ifPath;
}
//----------------------------------------------------------------------------------------------------------------------
bool FileEncoder::encodeInputFile() {
  return encodeMagicNum() && encodeBlocksCount() && encodeBlocks();
}
//----------------------------------------------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------------------------------------------
