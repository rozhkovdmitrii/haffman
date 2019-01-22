//----------------------------------------------------------------------------------------------------------------------
#include <iostream>
//----------------------------------------------------------------------------------------------------------------------
#include "FileDecoder.h"
//----------------------------------------------------------------------------------------------------------------------
int main(int argc, char *argv[]) {
  const char * msg = "'decode' -- decodes input file to the original one:\n"
                     "\n"
                     "> decode <input-file> <output-file>";

  if (argc !=  3) {
    std::cout << msg << std::endl << "Should be 2 arguments. Passed: " << argc - 1 << std::endl;
    return 1;
  }

  Haffman::FileDecoder decoder(argv[1], argv[2]);

  if (!decoder.isReadyToDecode()) {
    std::cout << std::endl << msg << std::endl;
    LOG(APPERR) << "Should be 2 arguments. Passed: " << argc - 1 << "\n";
    return 1;
  }

  if (!decoder.decodeInputFile())
    return 1;

  LOG(APPINF) << "In file successfully decoded: \"" << decoder.getIfPath() << "\"";
  return 0;
}
//----------------------------------------------------------------------------------------------------------------------
