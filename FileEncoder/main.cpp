//----------------------------------------------------------------------------------------------------------------------
#include <iostream>
//----------------------------------------------------------------------------------------------------------------------
#include "FileEncoder.h"
//----------------------------------------------------------------------------------------------------------------------
int main(int argc, char *argv[]) {
  if (argc !=  3)
    return LOG(APPERR) << "Should be 2 arguments. Passed: " << argc - 1;

  Log::setEnabled(Log::Type::DBGERR, true);
  Log::setEnabled(Log::Type::APPERR, true);

  Haffman::FileEncoder encoder(argv[1], argv[2]);
  if (!encoder.encodeInputFile())
    return 1;

  LOG(APPINF) << "In file successfully encoded: \"" << encoder.getIfPath() << "\"";
  return 0;
}
//----------------------------------------------------------------------------------------------------------------------
