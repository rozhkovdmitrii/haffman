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
  virtual ~TreeNode() = default;

  enum class Type {
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
typedef std::vector<TreeNode *> VecTreeNodePtr;
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
}
#endif //HAFFMAN_TREENODE_H
