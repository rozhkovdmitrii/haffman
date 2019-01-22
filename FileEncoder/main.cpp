//----------------------------------------------------------------------------------------------------------------------
#include <iostream>
//----------------------------------------------------------------------------------------------------------------------
#include "FileEncoder.h"
//----------------------------------------------------------------------------------------------------------------------
int main(int argc, char *argv[]) {
  const char * msg = "'encode' -- encodes any input file using Huffman algorithm:\n"
                     "\n"
                     "> encode <input-file> <output-file>";
  if (argc !=  3) {
    std::cout << msg << std::endl << "Should be 2 arguments. Passed: " << argc - 1 << std::endl;
    return 1;
  }

  Haffman::FileEncoder encoder(argv[1], argv[2]);
  if (!encoder.encodeInputFile())
    return 1;

  LOG(APPINF) << "In file successfully encoded: \"" << encoder.getIfPath() << "\"";
  return 0;
}
//----------------------------------------------------------------------------------------------------------------------
