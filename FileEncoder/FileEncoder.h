#ifndef HAFFMAN_FILEENCODER_H
#define HAFFMAN_FILEENCODER_H

#include <string>
#include <fstream>
#include <HaffmanEncoderImpl.h>

namespace Haffman
{
class FileEncoder {
public:
  enum {
    BlockSize = 1024 * 1024 * 2
  };

  explicit FileEncoder(const std::string & ifPath, const std::string & ofPath);
  operator bool() const;
  bool encodeInputFile();

  const std::string & getIfPath() const;

private:
  bool encodeMagicNum();
  bool encodeBlocksCount();
  bool encodeBlocks();
  bool encodeBlock();

  bool getInStreamSize(size_t & size);
  std::string _ifPath;
  std::ifstream _ifstream;
  std::string _ofPath;
  std::ofstream _ofstream;
  uint _blocksCount;
  HaffmanImpl::HaffmanEncoderImpl _haffmanEncoder;

  std::array<char, BlockSize> _buffer;
};
}

#endif //HAFFMAN_FILEENCODER_H
