#include <sstream>
#include <queue>
#include <algorithm>
#include <vector>
#include <iostream>
#include "HaffmanTree.h"

namespace Haffman
{

JoinNode::JoinNode(TreeNode * left, TreeNode * right) :
  TreeNode(Type::Join),
  _left(left),
  _right(right)
{
  calcFrequency();
}

JoinNode::~JoinNode()
{
  if (nullptr != dynamic_cast<JoinNode *>(_left))
    delete _left;
  if (nullptr != dynamic_cast<JoinNode *>(_right))
    delete _right;
}

void JoinNode::calcFrequency()
{
  _freq = (_left ? _left->getFreq() : 0) + (_right ? _right->getFreq() : 0);
}

void JoinNode::setLeft(TreeNode * node)
{
  _left = node;
  calcFrequency();
}

void JoinNode::setRight(TreeNode * node)
{
  _right = node;
  calcFrequency();
}

void JoinNode::setCode(const TreeCode & code)
{
  TreeNode::setCode(code);
  if (_left)
    _left->setCode(_code.getCodeToTheLeft());
  if (_right)
    _right->setCode(_code.getCodeToTheRight());
}

TreeNode * JoinNode::getLeft() const
{
  return _left;
}

TreeNode * JoinNode::getRight() const
{
  return _right;
}

std::string JoinNode::toString() const
{
  std::ostringstream ostream;
  ostream << "(" << (_left ? _left->toString() : "NULL") << "-" << (_right ? _right->toString() : "NULL") << ":"
          << _freq << ")";
  return ostream.str();
}

TreeNode::Type TreeNode::getType() const
{
  return _type;
}

int TreeNode::getFreq() const
{
  return _freq;
}

void TreeNode::setFreq(int freq)
{
  _freq = freq;
}

TreeCode TreeCode::getCodeToTheRight()
{
  TreeCode codeToTheRight(*this);
  codeToTheRight._base <<= 1;
  codeToTheRight._base |= 1;
  ++codeToTheRight._size;
  return codeToTheRight;
}

TreeCode TreeCode::getCodeToTheLeft()
{
  TreeCode codeToTheLeft(*this);
  codeToTheLeft._base <<= 1;
  ++codeToTheLeft._size;
  return codeToTheLeft;
}

TreeCode::TreeCode(const TreeCode & code) : _base(code._base), _size(code._size)
{
}

void TreeNode::setCode(const TreeCode & code)
{
  _code = code;
}

const TreeCode & TreeNode::getCode() const
{
  return _code;
}

std::string LeafNode::toString() const
{
  std::ostringstream ostream;
  ostream << "(" << _sym << _freq << ")";
  return ostream.str();
}

HaffmanTree::HaffmanTree(const VecLeafNodePtr & _freqTable) {
  buildTree(_freqTable);
  indexTree();
}

HaffmanTree::~HaffmanTree() {
  if (_top != nullptr)
    delete _top;
}

void HaffmanTree::buildTree(const VecLeafNodePtr & _freqTable)
{
  std::deque<TreeNode *> deque;
  for (auto i = 0; i < _freqTable.size(); ++i)
  {
    LeafNode * currLeaf = _freqTable[i];
    if (deque.empty())
      deque.push_back(currLeaf);
    else if (currLeaf->getFreq() >= deque.back()->getFreq() )
      deque.back() =  new JoinNode(currLeaf, deque.back());
    else
      deque.push_back(currLeaf);
  }

  _top = new JoinNode();
  for (auto i = deque.begin(); i != deque.end(); ++i)
  {
    if (!_top->getLeft())
      _top->setLeft(*i);
    else if (!_top->getRight())
      _top->setRight(*i);
    else
      _top = new JoinNode(_top, *i);
  }
}

void HaffmanTree::indexTree() {
  if (!_top)
  {
    std::cout << "EROOR: cannot index empty haffman tree." << std::endl;
    return;
  }
  _top->setCode(TreeCode());
}

void HaffmanTree::reset() {
  if (_top == nullptr)
    return;
  delete _top;
  _top = nullptr;
}

JoinNode * HaffmanTree::getTop() const {
  return _top;
}

std::string HaffmanTree::toString() const {
  return (_top == nullptr) ? "()" : _top->toString();
}


}