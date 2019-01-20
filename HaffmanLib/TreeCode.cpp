#include "TreeCode.h"
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
bool TreeCode::print() const {
  for (int i = 0; i < _size; ++i) { // TODO: relate to EncodeState
    int key = (_base >> (15 - i) & 1);
    std::cout << key;
  }
}
//----------------------------------------------------------------------------------------------------------------------

}