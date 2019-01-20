#ifndef HAFFMAN_TREECODE_H
#define HAFFMAN_TREECODE_H
#include <iostream>
#include "CommonTypes.h"
namespace HaffmanImpl
{
struct TreeCode
{
  ushort _base = 0;
  int _size = 0;
  TreeCode() = default;
  TreeCode(ushort base, int size);
  TreeCode(const TreeCode &);
  TreeCode getCodeToTheLeft() const;
  TreeCode getCodeToTheRight() const;
  bool operator==(const TreeCode & right) const;
  bool print() const;
};

}

#endif //HAFFMAN_TREECODE_H
