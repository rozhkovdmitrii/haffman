#ifndef HAFFMAN_TREENODE_H
#define HAFFMAN_TREENODE_H
//----------------------------------------------------------------------------------------------------------------------
#include "TreeCode.h"
//----------------------------------------------------------------------------------------------------------------------
namespace HaffmanImpl
{
//----------------------------------------------------------------------------------------------------------------------
class TreeNode {
public:
  enum class Type {
    Leaf,
    Join
  };

  TreeNode(Type type, uint freq) : _type(type), _freq(freq) {}
  explicit TreeNode(Type type) : TreeNode(type, 0) {}

  virtual ~TreeNode() = default;

  virtual void setCode(const TreeCode & code);

  Type getType() const;
  uint getFreq() const;
  virtual std::string toString() const = 0;

protected:
  uint _freq = 0;
  Type _type;
  TreeCode _code;
};
//----------------------------------------------------------------------------------------------------------------------
typedef std::vector<TreeNode *> VecTreeNodePtr;
//----------------------------------------------------------------------------------------------------------------------
class LeafNode : public TreeNode
{
public:
  LeafNode(byte sym, long freq) : TreeNode(Type::Leaf, freq), _sym(sym) {}
  LeafNode() : LeafNode(0, 0) {}

  ~LeafNode() override = default;

  std::string toString() const override;
  const TreeCode & getCode() const;
  const byte getSym() const;

private:
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
  bool setRight(TreeNode * node);

  TreeNode * getLeft() const;
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
}
#endif //HAFFMAN_TREENODE_H
