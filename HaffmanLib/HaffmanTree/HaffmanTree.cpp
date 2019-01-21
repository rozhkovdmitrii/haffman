//----------------------------------------------------------------------------------------------------------------------
#include <vector>
#include <queue>
#include <algorithm>
//----------------------------------------------------------------------------------------------------------------------
#include "Log.h"
#include "HaffmanTree.h"
//----------------------------------------------------------------------------------------------------------------------
namespace HaffmanImpl
{
//----------------------------------------------------------------------------------------------------------------------
HaffmanTree::HaffmanTree(const VecFreqItem & vecFreqItem) {

  resetFrom(vecFreqItem);
}

void HaffmanTree::resetFrom(const VecFreqItem & vecFreqItem) {
  buildFrom(vecFreqItem);
  updateCachedCodes(_top);
  indexTree();
}

//----------------------------------------------------------------------------------------------------------------------
HaffmanTree::~HaffmanTree() {
  delete _top;
}
//----------------------------------------------------------------------------------------------------------------------
void HaffmanTree::buildFrom(const VecFreqItem & vecFreqItem) {
  reset();

  VecTreeNodePtr vecTreeNodePtr;
  for (const auto & freqItemPtr : vecFreqItem) {
    auto * newLeaf = new LeafNode(freqItemPtr._sym, freqItemPtr._freq);
    vecTreeNodePtr.push_back(newLeaf);
  }
  auto cmp = [](const TreeNode * left, const TreeNode * right) -> bool {
    return left->getFreq() > right->getFreq();
  };
  std::priority_queue<TreeNode *, std::vector<TreeNode *>, decltype(cmp)> queue(cmp, vecTreeNodePtr);

  auto * join = new JoinNode;
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
//----------------------------------------------------------------------------------------------------------------------
void HaffmanTree::updateCachedCodes(TreeNode * treeNode) {
  if (treeNode == nullptr)
    return;
  auto * join = dynamic_cast<JoinNode *>(treeNode);
  if (join != nullptr)
  {
    updateCachedCodes(join->getLeft());
    updateCachedCodes(join->getRight());
    return;
  }
  auto * leaf = dynamic_cast<LeafNode *>(treeNode);
  if (leaf != nullptr)
    _rawLeafNodes[leaf->getSym()] = leaf;
}
//----------------------------------------------------------------------------------------------------------------------
void HaffmanTree::indexTree() {
  if (!_top)
  {
    std::cout << "EROOR: cannot index empty haffman tree." << std::endl;
    return;
  }
  _top->setCode(TreeCode());
}
//----------------------------------------------------------------------------------------------------------------------
void HaffmanTree::reset() {
  std::fill(_rawLeafNodes.begin(), _rawLeafNodes.end(), nullptr);
  delete _top;
  _top = nullptr;
}
//----------------------------------------------------------------------------------------------------------------------
JoinNode * HaffmanTree::getTop() const {
  return _top;
}
//----------------------------------------------------------------------------------------------------------------------
std::string HaffmanTree::toString() const {
  return (_top == nullptr) ? "()" : _top->toString();
}
//----------------------------------------------------------------------------------------------------------------------
const TreeCode & HaffmanTree::getCode(byte sym) const {
  const static TreeCode empty;
  if (_rawLeafNodes[sym] == nullptr)
  {
    LOG(DBGERR) << "Getting code fo sym '" << sym << "' failed";
    return empty;
  }
  return _rawLeafNodes[sym]->getCode();
}
//----------------------------------------------------------------------------------------------------------------------
HaffmanTree::HaffmanTree() : _top(nullptr) {}

//----------------------------------------------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------------------------------------------
