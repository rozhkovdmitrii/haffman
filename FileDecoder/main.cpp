//----------------------------------------------------------------------------------------------------------------------
#include <iostream>
//----------------------------------------------------------------------------------------------------------------------
#include "FileDecoder.h"
//----------------------------------------------------------------------------------------------------------------------
int main(int argc, char *argv[]) {
  if (argc !=  3)
    return LOG(APPERR) << "Should be 2 arguments. Passed: " << argc - 1;

  Log::setEnabled(Log::Type::DBGERR, false);
  Log::setEnabled(Log::Type::APPERR, true);

  Haffman::FileDecoder decoder(argv[1], argv[2]);
  if (!decoder.decodeInputFile())
    return 1;

  LOG(APPINF) << "In file successfully decoded: \"" << decoder.getIfPath() << "\"";
  return 0;
}
//----------------------------------------------------------------------------------------------------------------------
