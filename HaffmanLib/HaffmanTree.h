#ifndef HAFFMAN_HAFFMANTREE_H
#define HAFFMAN_HAFFMANTREE_H

#include <array>
#include "CommonTypes.h"

namespace Haffman
{

struct TreeCode
{
  byte _base = 0;
  int _size = 0;
  TreeCode() = default;
  TreeCode(byte base, int size);
  TreeCode(const TreeCode &);
  TreeCode getCodeToTheLeft() const;
  TreeCode getCodeToTheRight() const;
  bool operator==(const TreeCode & right) const;
};
typedef std::vector<TreeCode> VecTreeCode;
typedef std::vector<TreeCode &> VecTreeCodeRef;

std::ostream & operator<<(std::ostream& os, const TreeCode & treeCode);

class TreeNode
{
public:
  virtual ~TreeNode() {};

  enum class Type
  {
    Leafe,
    Join
  };

  Type getType() const;
  int getFreq() const;
  void setFreq(int freq);

  virtual void setCode(const TreeCode & code);
  virtual std::string toString() const = 0;

protected:
  TreeNode(Type type, long freq) : _type(type), _freq(freq) {}
  TreeNode(Type type) : TreeNode(type, 0) {}

  long _freq = 0;
  Type _type;
  TreeCode _code;
};

class LeafNode : public TreeNode
{
public:
  virtual ~LeafNode() override {}
  LeafNode(char sym, long freq) : TreeNode(Type::Leafe, freq), _sym(sym) {}
  LeafNode() : LeafNode(0, 0) {}
  virtual std::string toString() const override;
  const TreeCode & getCode() const;

  char _sym = 0;
};
typedef std::vector<LeafNode *> VecLeafNodePtr;

class JoinNode : public TreeNode
{
public:
  JoinNode() : JoinNode(nullptr, nullptr) {}
  JoinNode(TreeNode * left, TreeNode * right);
  virtual ~JoinNode() override;

  bool put(TreeNode * node);
  bool setLeft(TreeNode * node);
  TreeNode * getLeft() const;
  bool setRight(TreeNode * node);
  TreeNode * getRight() const;
  bool isFull() const;
  void calcFrequency();

  void setCode(const TreeCode & code) override;
  virtual std::string toString() const override ;

private:
  TreeNode * _left;
  TreeNode * _right;
};

class HaffmanTree
{
public:
  HaffmanTree();
  explicit HaffmanTree(const VecFreqItem & vecFreqItem);
  HaffmanTree(HaffmanTree && haffmanTree);
  HaffmanTree & operator=(HaffmanTree && haffmanTree) noexcept ;
  HaffmanTree(const HaffmanTree &) = delete;
  HaffmanTree & operator=(const  HaffmanTree &) = delete;

  ~HaffmanTree();
  JoinNode * getTop() const;
  std::string toString() const;
  const TreeCode & getCode(byte sym) const;

private:
  void buildTree(const VecLeafNodePtr & vecLeafNodePtr);
  void indexTree();
  void reset();

  std::array<LeafNode, 256> _rawLeafNodes;
  JoinNode * _top = nullptr;
};

}
#endif //HAFFMAN_HAFFMANTREE_H
