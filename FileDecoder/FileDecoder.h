//----------------------------------------------------------------------------------------------------------------------
#ifndef HAFFMAN_FILEDECODER_H
#define HAFFMAN_FILEDECODER_H
//----------------------------------------------------------------------------------------------------------------------
#include <string>
#include <fstream>
//----------------------------------------------------------------------------------------------------------------------
#include "HaffmanDecoderImpl.h"
//----------------------------------------------------------------------------------------------------------------------
namespace Haffman
{
//----------------------------------------------------------------------------------------------------------------------
class FileDecoder {
public:
  enum {
    BlockSize = 1024 * 1024 * 5
  };

  explicit FileDecoder(const std::string & ifPath, const std::string & ofPath);
  bool decodeInputFile();
  const std::string & getIfPath() const;
  bool isReadyToDecode() const;

private:
  bool decodeMaginNum();
  bool decodeBlocks();

  std::string _ifPath;
  std::ifstream _ifstream;
  std::string _ofPath;
  std::ofstream _ofstream;
  uint _blocksCount;
  HaffmanImpl::HaffmanDecoderImpl _haffmanDecoder;

  std::array<char, BlockSize> _buffer;
};
//----------------------------------------------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------------------------------------------
#endif //HAFFMAN_FILEDECODER_H
//----------------------------------------------------------------------------------------------------------------------
