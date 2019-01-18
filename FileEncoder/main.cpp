//----------------------------------------------------------------------------------------------------------------------
#include <iostream>
#include "FileEncoder.h"
//----------------------------------------------------------------------------------------------------------------------
int main(int argc, char *argv[]) {
  ErrLog::setEnabled(ErrLog::Type::DBGERR, false);
  ErrLog::setEnabled(ErrLog::Type::APPERR, true);

  Haffman::FileEncoder encoder(argv[1], argv[2]);
  if (!encoder)
    return 1;
  encoder.encodeMagicNum();
  encoder.encodeBlocksCount();
  encoder.encodeBlocks();
}
//----------------------------------------------------------------------------------------------------------------------
