#ifndef HAFFMAN_COMMONTYPES_H
#define HAFFMAN_COMMONTYPES_H

#include <vector>
#include <array>


typedef unsigned char byte;

namespace HaffmanImpl
{



struct FreqItem {
  byte _sym;
  unsigned int _freq;
  FreqItem() : FreqItem(0, 0) {}
  FreqItem(byte sym, unsigned int freq) : _sym(sym), _freq(freq) {}
};



}

typedef std::vector<HaffmanImpl::FreqItem> VecFreqItem;
typedef std::vector<unsigned int> VecUInt;
typedef std::array<unsigned int, 256> ArrUInt256;
typedef std::vector<byte> VecByte;

#endif //HAFFMAN_COMMONTYPES_H
