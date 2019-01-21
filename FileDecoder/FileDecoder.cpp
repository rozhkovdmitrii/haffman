//----------------------------------------------------------------------------------------------------------------------
#include "FileDecoder.h"
//----------------------------------------------------------------------------------------------------------------------
namespace Haffman
{

FileDecoder::FileDecoder(const std::string & ifPath, const std::string & ofPath) :
  _ifPath(ifPath),
  _ofPath(ofPath),
  _ifstream(ifPath,  std::ios::binary),
  _ofstream(ofPath, std::ios::binary)
{
  _ifstream.seekg (0, std::ios::beg);
}
//----------------------------------------------------------------------------------------------------------------------
bool FileDecoder::decodeInputFile() {
  return isReadyToDecode() && decodeMaginNum() && decodeBlocks();
}
//----------------------------------------------------------------------------------------------------------------------
bool FileDecoder::isReadyToDecode() const {
  if (!_ifstream.good())
    return LOG(APPERR) << "Input file is not good: " << _ifPath;

  if (!_ofstream.good())
    return LOG(APPERR) << "Out file is not good: " << _ofPath;
  return true;
}
//----------------------------------------------------------------------------------------------------------------------
bool FileDecoder::decodeMaginNum() {
  unsigned long long magicNum = 0;
  if ( _ifstream.readsome(reinterpret_cast<char *>(&magicNum), sizeof(magicNum)) != sizeof(magicNum))
    return LOG(APPERR) << "Reading MagicNumber failed";
  if (magicNum != HaffmanImpl::MagicNum)
    return LOG(APPERR) << "Wrong input file format";
  return true;
}
//----------------------------------------------------------------------------------------------------------------------
bool FileDecoder::decodeBlocks() {

  while (!_haffmanDecoder.isFinished())
  {
    uint readedCount = (uint) _ifstream.readsome(_buffer.data(), BlockSize);
    if (readedCount == 0)
      return LOG(APPERR) << "Reading next block failed, can not read more, but data expected";

    VecByte decBuffer;
    _haffmanDecoder.addDataAndTryToDecode(_buffer.begin(), _buffer.begin() + readedCount, decBuffer);
    if (_haffmanDecoder.isError())
     return LOG(APPERR) << "Decoding failed";
    if (!decBuffer.empty() && !_ofstream.write(reinterpret_cast<const char *>(decBuffer.data()), decBuffer.size()))
      return LOG(APPERR) << "Write decoded data into out file failed";
    _ofstream.flush();
  }

  return true;
}
//----------------------------------------------------------------------------------------------------------------------
const std::string & Haffman::FileDecoder::getIfPath() const {
  return _ifPath;
}
//----------------------------------------------------------------------------------------------------------------------
}