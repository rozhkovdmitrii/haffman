#include <iostream>
#include <sstream>
#include <vector>
#include <queue>
#include <algorithm>

#include "Log.h"
#include "HaffmanTree.h"

namespace HaffmanImpl
{

JoinNode::JoinNode(TreeNode * left, TreeNode * right) :
  TreeNode(Type::Join),
  _left(left), _right(right)
{
  calcFrequency();
}

JoinNode::JoinNode() : JoinNode(nullptr, nullptr) {
}


JoinNode::~JoinNode()
{
  delete _left;
  delete _right;
}

void JoinNode::calcFrequency()
{
  _freq = (_left ? _left->getFreq() : 0) + (_right ? _right->getFreq() : 0);
}

bool JoinNode::setLeft(TreeNode * node)
{
  if (_left != nullptr)
    return false;
  _left = node;
  calcFrequency();
  return true;
}

bool JoinNode::setRight(TreeNode * node)
{
  if (_right != nullptr)
    return false;
  _right = node;
  calcFrequency();
  return true;
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

bool JoinNode::put(TreeNode * node) {
  if (node == nullptr)
    return LOG(DBGERR) << "cannot put nullptr node into HaffmanImpl Tree";
  if (setLeft(node) || setRight(node))
    return true;
  return LOG(DBGERR) << "cannot put node into full HaffmanImpl Tree";
}

bool JoinNode::isFull() const {
  return _left != nullptr && _right != nullptr;
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

TreeCode::TreeCode(byte base, int size) : _base(base), _size(size) {}
TreeCode::TreeCode(const TreeCode & code) : TreeCode(code._base, code._size) {}

TreeCode TreeCode::getCodeToTheRight() const {
  TreeCode codeToTheRight(*this);
  codeToTheRight._base <<= 1;
  codeToTheRight._base |= 1;
  ++codeToTheRight._size;
  return codeToTheRight;
}

TreeCode TreeCode::getCodeToTheLeft() const {
  TreeCode codeToTheLeft(*this);
  codeToTheLeft._base <<= 1;
  ++codeToTheLeft._size;
  return codeToTheLeft;
}

bool TreeCode::operator==(const TreeCode & right) const {
  return _size == right._size && _base == right._base;
}

void TreeNode::setCode(const TreeCode & code) {
  _code = code;
}

std::string LeafNode::toString() const {
  std::ostringstream ostream;
  ostream << "(" << _sym << _freq << ")";
  return ostream.str();
}

const TreeCode & LeafNode::getCode() const {
  return _code;
}

const byte LeafNode::getSym() const {
  return _sym;
}

void LeafNode::setSym(byte sym) {
  _sym = sym;
}

HaffmanTree::HaffmanTree(const VecFreqItem & vecFreqItem) {

  buildFrom(vecFreqItem);
  updateCachedCodes(_top);
  indexTree();
}

HaffmanTree::~HaffmanTree() {
  if (_top != nullptr)
    delete _top;
}

void HaffmanTree::buildFrom(const VecFreqItem & vecFreqItem) {
  reset();

  VecTreeNodePtr vecTreeNodePtr;
  for (const auto & freqItemPtr : vecFreqItem) {
    LeafNode * newLeaf = new LeafNode(freqItemPtr._sym, freqItemPtr._freq);
    vecTreeNodePtr.push_back(newLeaf);
  }
  auto cmp = [](const TreeNode * left, const TreeNode * right) -> bool {
    return left->getFreq() > right->getFreq();
  };
  std::priority_queue<TreeNode *, std::vector<TreeNode *>, decltype(cmp)> queue(cmp, vecTreeNodePtr);

  JoinNode * join = new JoinNode;
  while (!queue.empty()) {
    TreeNode * queueTop = queue.top();
    if (join->getFreq() > queueTop->getFreq()) {
      queue.push(join);
      join = new JoinNode;
      continue;
    }
    queue.pop();
    if (join->isFull())
      join = new JoinNode(join, nullptr);
    join->put(queueTop);
  }
  _top = join;
}

void HaffmanTree::updateCachedCodes(TreeNode * treeNode) {
  if (treeNode == nullptr)
    return;
  JoinNode * join = dynamic_cast<JoinNode *>(treeNode);
  if (join != nullptr)
  {
    updateCachedCodes(join->getLeft());
    updateCachedCodes(join->getRight());
    return;
  }
  LeafNode * leaf = dynamic_cast<LeafNode *>(treeNode);
  if (leaf != nullptr)
    _rawLeafNodes[leaf->getSym()] = leaf;
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
  std::fill(_rawLeafNodes.begin(), _rawLeafNodes.end(), nullptr);
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

const TreeCode & HaffmanTree::getCode(byte sym) const {
  const static TreeCode empty;
  return _rawLeafNodes[sym] == nullptr ? empty : _rawLeafNodes[sym]->getCode();
}

HaffmanTree::HaffmanTree() : _top(nullptr) {}

#include <algorithm>
HaffmanTree::HaffmanTree(HaffmanTree && haffmanTree) {
  reset();
  _top = haffmanTree._top;
  updateCachedCodes(_top);
  haffmanTree._top = nullptr;
  std::copy(haffmanTree._rawLeafNodes.begin(), haffmanTree._rawLeafNodes.end(), _rawLeafNodes.begin());
}

HaffmanTree & HaffmanTree::operator=(HaffmanTree && haffmanTree) noexcept {
  reset();
  _top = haffmanTree._top;
  updateCachedCodes(_top);
  haffmanTree._top = nullptr;
  return *this;
}


std::ostream & operator<<(std::ostream & os, const TreeCode & treeCode) {
  int size = treeCode._size;
  while (size)
  {
    --size;
    os << (bool)(treeCode._base & 1 << size);
  }
  return os;
}
}