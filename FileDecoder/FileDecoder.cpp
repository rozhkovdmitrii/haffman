#include "FileDecoder.h"

Haffman::FileDecoder::FileDecoder(const std::string & ifPath, const std::string & ofPath) :
  _ifPath(ifPath),
  _ofPath(ofPath),
  _ifstream(ifPath,  std::ios::binary),
  _ofstream(ofPath, std::ios::binary)
{

}

bool Haffman::FileDecoder::decodeInputFile() {
  return true;
}

const std::string & Haffman::FileDecoder::getIfPath() const {
  return _ifPath;
}
