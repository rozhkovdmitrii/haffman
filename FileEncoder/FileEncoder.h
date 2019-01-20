//----------------------------------------------------------------------------------------------------------------------
#ifndef HAFFMAN_FILEENCODER_H
#define HAFFMAN_FILEENCODER_H
//----------------------------------------------------------------------------------------------------------------------
#include <string>
#include <fstream>
//----------------------------------------------------------------------------------------------------------------------
#include "HaffmanEncoderImpl.h"
//----------------------------------------------------------------------------------------------------------------------
namespace Haffman
{
class FileEncoder {
public:
  enum {
    BlockSize = 1024 * 1024 * 5
  };

  explicit FileEncoder(const std::string & ifPath, const std::string & ofPath);
  bool encodeInputFile();
  const std::string & getIfPath() const;

private:
  bool isReadyToEncode() const;
  bool encodeMagicNum();
  bool encodeBlocksCount();
  bool encodeBlocks();
  bool encodeBlock(char * from, char * to);

  bool getInStreamSize(size_t & size);
  std::string _ifPath;
  std::ifstream _ifstream;
  std::string _ofPath;
  std::ofstream _ofstream;
  uint _blocksCount;
  HaffmanImpl::HaffmanEncoderImpl _haffmanEncoder;

  std::array<byte, BlockSize> _buffer;
};
}
//----------------------------------------------------------------------------------------------------------------------
#endif //HAFFMAN_FILEENCODER_H
//----------------------------------------------------------------------------------------------------------------------
