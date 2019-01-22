//----------------------------------------------------------------------------------------------------------------------
#include <iostream>
#include <iomanip>
//----------------------------------------------------------------------------------------------------------------------
#include "TreeCode.h"
//----------------------------------------------------------------------------------------------------------------------
namespace HaffmanImpl
{
//----------------------------------------------------------------------------------------------------------------------
TreeCode::TreeCode(ushort base, int size) : _base(base), _size(size) {}
//----------------------------------------------------------------------------------------------------------------------
TreeCode::TreeCode(const TreeCode & code) : TreeCode(code._base, code._size) {}
//----------------------------------------------------------------------------------------------------------------------
TreeCode TreeCode::getCodeToTheLeft() const {
  TreeCode codeToTheLeft(*this);
  codeToTheLeft._base <<= 1;
  ++codeToTheLeft._size;
  return codeToTheLeft;
}
//----------------------------------------------------------------------------------------------------------------------
TreeCode TreeCode::getCodeToTheRight() const {
  TreeCode codeToTheRight(*this);
  codeToTheRight._base <<= 1;
  codeToTheRight._base |= 1;
  ++codeToTheRight._size;
  return codeToTheRight;
}
//----------------------------------------------------------------------------------------------------------------------
bool TreeCode::operator==(const TreeCode & right) const {
  return _size == right._size && _base == right._base;
}
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
bool WriteTreeCodeState::isEmpty() const {
  return _bufferedCount == 0;
}
//----------------------------------------------------------------------------------------------------------------------
bool WriteTreeCodeState::putAndCheckPossibleToWrite(const TreeCode & treeCode, ushort & toBeWrote) {
  int rest = DigitCount - _bufferedCount;
  if (rest > treeCode._size) {
    ushort valToBuf = treeCode._base << (rest - treeCode._size);
    _buffer |= valToBuf;
    _bufferedCount += treeCode._size;
    return false;
  }
  toBeWrote = _buffer | treeCode._base >> (treeCode._size - rest);
  _buffer = treeCode._base << (DigitCount - treeCode._size + rest);
  _bufferedCount = treeCode._size - rest;
  return true;
}
//----------------------------------------------------------------------------------------------------------------------
std::ostream & operator<<(std::ostream & ostream, const TreeCode & code) {
  ostream << "{" << std::hex << "0x" << std::setw(2) << std::setfill('0') << code._base << ":" << code._size << "}";
  return ostream;
}
//----------------------------------------------------------------------------------------------------------------------
}