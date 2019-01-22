//----------------------------------------------------------------------------------------------------------------------
#ifndef HAFFMAN_HAFFMANTREE_H
#define HAFFMAN_HAFFMANTREE_H
//----------------------------------------------------------------------------------------------------------------------
#include <array>
#include "CommonTypes.h"
#include "TreeNode.h"
//----------------------------------------------------------------------------------------------------------------------
namespace HaffmanImpl
{
//----------------------------------------------------------------------------------------------------------------------
class HaffmanTree
{
public:
  HaffmanTree();
  explicit HaffmanTree(const VecFreqItem & vecFreqItem);

  HaffmanTree(HaffmanTree && haffmanTree) noexcept = delete;
  HaffmanTree & operator=(HaffmanTree && haffmanTree) noexcept = delete;
  HaffmanTree(const HaffmanTree &) = delete;
  HaffmanTree & operator=(const  HaffmanTree &) = delete;

  ~HaffmanTree();
  JoinNode * getTop() const;

  const TreeCode & getCode(byte sym) const;
  void resetFrom(const VecFreqItem & vecFreqItem);
  void reset();

private:
  void buildFrom(const VecFreqItem & vecFreqItem);
  void updateCachedCodes(TreeNode *);
  void indexTree();

  JoinNode * _top = nullptr;
  std::array<LeafNode *, 256> _rawLeafNodes;
};
//----------------------------------------------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------------------------------------------
#endif //HAFFMAN_HAFFMANTREE_H
//----------------------------------------------------------------------------------------------------------------------
