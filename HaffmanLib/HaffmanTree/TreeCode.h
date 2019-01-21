//----------------------------------------------------------------------------------------------------------------------
#ifndef HAFFMAN_TREECODE_H
#define HAFFMAN_TREECODE_H
//----------------------------------------------------------------------------------------------------------------------
#include "CommonTypes.h"
//----------------------------------------------------------------------------------------------------------------------
namespace HaffmanImpl
{
//----------------------------------------------------------------------------------------------------------------------
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
};
//----------------------------------------------------------------------------------------------------------------------
typedef std::vector<TreeCode> VecTreeCode;
//----------------------------------------------------------------------------------------------------------------------
std::ostream & operator<<(std::ostream& os, const TreeCode & treeCode);
//----------------------------------------------------------------------------------------------------------------------
struct WriteTreeCodeState {
  ushort _buffer = 0;

  enum {
    DigitCount = sizeof(_buffer) * 8
  };

  byte _bufferedCount = 0;
  bool isEmpty() const { return _bufferedCount == 0; }
  bool putAndCheckPossibleToWrite(const TreeCode & treeCode, ushort & toBeWrote);
};
//----------------------------------------------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------------------------------------------
#endif //HAFFMAN_TREECODE_H
//----------------------------------------------------------------------------------------------------------------------
