//----------------------------------------------------------------------------------------------------------------------
#ifndef HAFFMAN_HAFFMANTREE_H
#define HAFFMAN_HAFFMANTREE_H
//----------------------------------------------------------------------------------------------------------------------
#include <array>
#include "CommonTypes.h"
#include "TreeCode.h"
//----------------------------------------------------------------------------------------------------------------------
namespace HaffmanImpl
{
//----------------------------------------------------------------------------------------------------------------------
typedef std::vector<TreeCode> VecTreeCode;
//----------------------------------------------------------------------------------------------------------------------
std::ostream & operator<<(std::ostream& os, const TreeCode & treeCode);
//----------------------------------------------------------------------------------------------------------------------
class TreeNode
{
public:
  virtual ~TreeNode() = default;

  enum class Type
  {
    Leaf,
    Join
  };

  Type getType() const;
  uint getFreq() const;
  void setFreq(uint freq);

  virtual void setCode(const TreeCode & code);
  virtual std::string toString() const = 0;

protected:
  TreeNode(Type type, uint freq) : _type(type), _freq(freq) {}
  explicit TreeNode(Type type) : TreeNode(type, 0) {}

  uint _freq = 0;
  Type _type;
  TreeCode _code;
};
//----------------------------------------------------------------------------------------------------------------------
typedef std::vector<TreeNode*> VecTreeNodePtr;
//----------------------------------------------------------------------------------------------------------------------
class LeafNode : public TreeNode
{
public:
  ~LeafNode() override = default;
  LeafNode(byte sym, long freq) : TreeNode(Type::Leaf, freq), _sym(sym) {}
  LeafNode() : LeafNode(0, 0) {}
  std::string toString() const override;
  const TreeCode & getCode() const;
  const byte getSym() const;
  void setCode(const TreeCode & code) override;
  void setSym(byte sym);
  byte _sym = 0;
};
//----------------------------------------------------------------------------------------------------------------------
typedef std::vector<LeafNode *> VecLeafNodePtr;
//----------------------------------------------------------------------------------------------------------------------
class JoinNode : public TreeNode
{
public:
  JoinNode();
  JoinNode(TreeNode * left, TreeNode * right);
  ~JoinNode() override;

  bool put(TreeNode * node);
  bool setLeft(TreeNode * node);
  TreeNode * getLeft() const;
  bool setRight(TreeNode * node);
  TreeNode * getRight() const;
  bool isFull() const;
  void calcFrequency();

  void setCode(const TreeCode & code) override;
  std::string toString() const override ;

private:
  TreeNode * _left;
  TreeNode * _right;
};
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
  std::string toString() const;
  void printCodes() const;
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
