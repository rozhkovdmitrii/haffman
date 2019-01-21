#ifndef HAFFMAN_COMMONTYPES_H
#define HAFFMAN_COMMONTYPES_H

#include <vector>
#include <array>


typedef unsigned char byte;
typedef unsigned short ushort;

namespace HaffmanImpl
{

struct FreqItem {
  byte _sym;
  uint _freq;
  FreqItem() : FreqItem(0, 0) {}
  FreqItem(byte sym, uint freq) : _sym(sym), _freq(freq) {}
  bool operator==(const FreqItem & freqItem) const {
    return _sym == freqItem._sym && _freq == freqItem._freq;
  }
};

const unsigned long long MagicNum = 0xFEDCBA98FEDCBA98;

}

typedef std::vector<HaffmanImpl::FreqItem> VecFreqItem;
typedef std::vector<unsigned int> VecUInt;
typedef std::array<unsigned int, 256> ArrUInt256;
typedef std::vector<byte> VecByte;


#endif //HAFFMAN_COMMONTYPES_H
