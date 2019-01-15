#ifndef HAFFMAN_HAFFMANTREE_H
#define HAFFMAN_HAFFMANTREE_H

namespace Haffman
{

struct TreeCode
{
  char _base = 0;
  int _size = 0;
  TreeCode() = default;
  TreeCode(const TreeCode &);
  TreeCode getCodeToTheLeft();
  TreeCode getCodeToTheRight();
};

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
  const TreeCode & getCode() const;

protected:
  TreeNode(Type type, int freq) : _type(type), _freq(freq) {}
  TreeNode(Type type) : TreeNode(type, 0) {}

  int _freq = 0;
  Type _type;
  TreeCode _code;
};

class LeafNode : public TreeNode
{
public:
  virtual ~LeafNode() override {}
  LeafNode(char sym, int freq) : TreeNode(Type::Leafe, freq), _sym(sym) {}
  LeafNode() : LeafNode(0, 0) {}

  bool operator<(const LeafNode & right)
  {
    return _freq < right._freq;
  };

  char _sym = 0;
  virtual std::string toString() const override;
};
typedef std::vector<LeafNode *> VecLeafNodePtr;

class JoinNode : public TreeNode
{
public:
  JoinNode() : JoinNode(nullptr, nullptr) {}
  JoinNode(TreeNode * left, TreeNode * right);
  virtual ~JoinNode() override;

  void setLeft(TreeNode * node);
  TreeNode * getLeft() const;
  void setRight(TreeNode * node);
  TreeNode * getRight() const;

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
  explicit HaffmanTree(const VecLeafNodePtr & _freqTable);
  ~HaffmanTree();
  JoinNode * getTop() const;
  std::string toString() const;

private:
  void buildTree(const VecLeafNodePtr & _freqTable);
  void indexTree();
  void reset();

  JoinNode * _top = nullptr;
};

}
#endif //HAFFMAN_HAFFMANTREE_H
