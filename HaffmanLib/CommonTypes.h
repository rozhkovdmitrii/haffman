#ifndef HAFFMAN_COMMONTYPES_H
#define HAFFMAN_COMMONTYPES_H

#include <vector>

namespace Haffman
{

typedef unsigned char byte;

struct FreqItem {
  byte _sym;
  unsigned int _freq;
  FreqItem() : FreqItem(0, 0) {}
  FreqItem(byte sym, unsigned int freq) : _sym(sym), _freq(freq) {}
};

typedef std::vector<FreqItem> VecFreqItem;
typedef std::vector<byte> VecByte;

}

#endif //HAFFMAN_COMMONTYPES_H
