#ifndef HAFFMAN_COMMONTYPES_H
#define HAFFMAN_COMMONTYPES_H

#include <vector>

namespace Haffman
{

struct FreqItem {
  char _sym = 0;
  long _freq = 0;
};

typedef std::vector<FreqItem *> VecFreqItemPtr;
typedef unsigned char byte;

}

#endif //HAFFMAN_COMMONTYPES_H
