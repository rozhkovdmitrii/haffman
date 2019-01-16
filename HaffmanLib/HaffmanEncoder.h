#ifndef HAFFMAN_HAFFMANENCODER_H
#define HAFFMAN_HAFFMANENCODER_H

#include "FrequencyTable.h"

namespace Haffman
{
class HaffmanEncoder
{
public:
  HaffmanEncoder() = default;

  template <typename T>
  bool encode(T begin, T end, VecByte & buffer);
  bool encode(const VecFreqItem &, VecByte & buffer);
  bool encode(const FreqItem &, VecByte & buffer);

  template <typename T>
  void write(T value, VecByte & buffer) const;

private:

  enum {
    ReadByCount = 50
  };
};
}

#endif //HAFFMAN_HAFFMANENCODER_H
