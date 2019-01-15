#ifndef HAFFMAN_HAFFMANENCODER_H
#define HAFFMAN_HAFFMANENCODER_H

#include "FrequencyTable.h"

namespace Haffman
{
class HaffmanEncoder
{
  HaffmanEncoder() = default;

  bool readFreqTable(const std::string filename);

private:
  FrequencyTable _freqTable;
};
}

#endif //HAFFMAN_HAFFMANENCODER_H
