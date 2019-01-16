#include <cmath>
#include "gtest/gtest.h"
#include "FrequencyTable.h"
#include "HaffmanEncoder.h"

using namespace Haffman;

class FrequencyTest : public testing::Test {
protected:
  virtual void SetUp() {
    _frequencyTable = new Haffman::FrequencyTable;
  };

  virtual void TearDown() {
    if (_frequencyTable != nullptr) delete _frequencyTable;
  };

  Haffman::FrequencyTable * _frequencyTable;
};

TEST_F(FrequencyTest, FrequencyTest_TestReadingSimple_Test) {
  const std::string seq = "beep boop beer!";
  _frequencyTable->takeFrequency((byte *) seq.data(), ((byte *) seq.data()) + seq.size());
  EXPECT_EQ(4, _frequencyTable->getFrequencyOf('e'));
  EXPECT_EQ(3, _frequencyTable->getFrequencyOf('b'));
  EXPECT_EQ(2, _frequencyTable->getFrequencyOf('p'));
  EXPECT_EQ(2, _frequencyTable->getFrequencyOf(' '));
  EXPECT_EQ(1, _frequencyTable->getFrequencyOf('r'));
  EXPECT_EQ(1, _frequencyTable->getFrequencyOf('!'));
  EXPECT_EQ(2, _frequencyTable->getFrequencyOf('o'));
}

TEST_F(FrequencyTest, SymFreqInitTest) {
  Haffman::LeafNode sf;
  EXPECT_EQ(0, sf.getFreq());
  EXPECT_EQ(0, sf.getFreq());
}

TEST_F(FrequencyTest, TreeCodeInitTest) {
  TreeCode treeCode;
  EXPECT_EQ(0, treeCode._base);
  EXPECT_EQ(0, treeCode._size);
}

TEST_F(FrequencyTest, SymFreqLessTest) {
  std::vector<Haffman::LeafNode> vec = {{'d', 5}, {'c', 1}, {'b', 1}};
  std::sort(vec.begin(), vec.end(), [](const Haffman::LeafNode & left, const Haffman::LeafNode & right) -> bool { return left.getFreq() > right.getFreq(); });
  EXPECT_EQ('d', vec[0]._sym);
  EXPECT_EQ('c', vec[1]._sym);
  EXPECT_EQ('b', vec[2]._sym);
}

TEST_F(FrequencyTest, EmptyDataTest) {

}

TEST_F(FrequencyTest, FrequencyTest_Test) {
  const std::string seq1 = "beep b";
  const std::string seq2 = "oop beer!";
  _frequencyTable->takeFrequency(seq1.cbegin(), seq1.cend());
  _frequencyTable->takeFrequency(seq2.cbegin(), seq2.cend());
  HaffmanTree haffmanTree = _frequencyTable->getTree();

  ASSERT_NE(nullptr, haffmanTree.getTop());
  EXPECT_EQ(TreeNode::Type ::Join, haffmanTree.getTop()->getType());
  EXPECT_EQ(15, haffmanTree.getTop()->getFreq());

  JoinNode * p0 = dynamic_cast<JoinNode *>(haffmanTree.getTop()->getLeft());
  ASSERT_NE(nullptr, p0);
  EXPECT_EQ(TreeNode::Type::Join, p0->getType());
  EXPECT_EQ(7, p0->getFreq());

  LeafNode * p00 = dynamic_cast<LeafNode *>(p0->getLeft());
  ASSERT_NE(nullptr, p00);
  EXPECT_EQ(TreeNode::Type::Leafe, p00->getType());
  EXPECT_EQ('b', p00->_sym);

  JoinNode * p01 = dynamic_cast<JoinNode *>(p0->getRight());
  ASSERT_NE(nullptr, p01);
  EXPECT_EQ(TreeNode::Type::Join, p01->getType());

  LeafNode * p011 = dynamic_cast<LeafNode *>(p01->getRight());
  ASSERT_NE(nullptr, p011);
  EXPECT_EQ(TreeNode::Type::Leafe, p011->getType());
  EXPECT_EQ('p', p011->_sym);

  JoinNode * p010 = dynamic_cast<JoinNode *>(p01->getLeft());
  ASSERT_NE(nullptr, p010);

  LeafNode * p0100 = dynamic_cast<LeafNode *>(p010->getLeft());
  ASSERT_NE(nullptr, p0100);
  EXPECT_EQ(TreeNode::Type::Leafe, p0100->getType());
  EXPECT_EQ('r', p0100->_sym);

  LeafNode * p0101 = dynamic_cast<LeafNode *>(p010->getRight());
  ASSERT_NE(nullptr, p0101);
  EXPECT_EQ(TreeNode::Type::Leafe, p0101->getType());
  EXPECT_EQ('!', p0101->_sym);

  JoinNode * p1 = dynamic_cast<JoinNode *>(haffmanTree.getTop()->getRight());
  ASSERT_NE(nullptr, p1);

  LeafNode * p10 = dynamic_cast<LeafNode *>(p1->getLeft());
  ASSERT_NE(nullptr, p10);
  EXPECT_EQ(TreeNode::Type::Leafe, p10->getType());
  EXPECT_EQ('e', p10->_sym);

  JoinNode * p11 = dynamic_cast<JoinNode *>(p1->getRight());
  ASSERT_NE(nullptr, p11);
  
  LeafNode * p111 = dynamic_cast<LeafNode *>(p11->getRight());
  ASSERT_NE(nullptr, p111);
  EXPECT_EQ(TreeNode::Type::Leafe, p111->getType());
  EXPECT_EQ('o', p111->_sym);

  LeafNode * p110 = dynamic_cast<LeafNode *>(p11->getLeft());
  ASSERT_NE(nullptr, p110);
  EXPECT_EQ(TreeNode::Type::Leafe, p110->getType());
  EXPECT_EQ(' ', p110->_sym);
}

TEST_F(FrequencyTest, FrequencyTable_TreeCodeTest_Test) {
  TreeCode base;
  TreeCode p0 = base.getCodeToTheLeft();
  EXPECT_EQ(0, p0._base);
  EXPECT_EQ(1, p0._size);

  TreeCode p1 = base.getCodeToTheRight();
  EXPECT_EQ(1, p1._base);
  EXPECT_EQ(1, p1._size);

  TreeCode p10 = p1.getCodeToTheLeft();
  EXPECT_EQ(2, p10._base);
  EXPECT_EQ(2, p10._size);

  TreeCode p11 = p1.getCodeToTheRight();
  EXPECT_EQ(3, p11._base);
  EXPECT_EQ(2, p11._size);

  TreeCode p001100 = p0.getCodeToTheLeft().getCodeToTheRight().getCodeToTheRight().getCodeToTheLeft().getCodeToTheLeft();
  EXPECT_EQ(12, p001100._base);
  EXPECT_EQ(6, p001100._size);
}

TEST_F(FrequencyTest, FrequencyTest_TreeReadingPossibleAfterReseting_Test) {
  const std::string seq = "beep boop beer!";
  _frequencyTable->takeFrequency(seq.cbegin(), seq.cend());

  HaffmanTree haffmanTree = _frequencyTable->getTree();
  EXPECT_NE(nullptr, haffmanTree.getTop());

  EXPECT_EQ(TreeCode({0, 0}), haffmanTree.getCode('z'));
  EXPECT_EQ(TreeCode({0, 2}), haffmanTree.getCode('b'));
  EXPECT_EQ(TreeCode({2, 2}), haffmanTree.getCode('e'));
  EXPECT_EQ(TreeCode({3, 3}), haffmanTree.getCode('p'));
  EXPECT_EQ(TreeCode({6, 3}), haffmanTree.getCode(' '));
  EXPECT_EQ(TreeCode({7, 3}), haffmanTree.getCode('o'));

  const std::string seq2 = "mama mila ramu";
  auto size = strlen(seq2.c_str());
  _frequencyTable->reset();
  _frequencyTable->takeFrequency(seq2.cbegin(), seq2.cend());

  HaffmanTree haffmanTree1 = _frequencyTable->getTree();
  ASSERT_NE(nullptr, haffmanTree1.getTop());
  EXPECT_EQ(TreeNode::Type ::Join, haffmanTree1.getTop()->getType());
  EXPECT_EQ(14, haffmanTree1.getTop()->getFreq());

}

TEST_F(FrequencyTest, EncodeFreqItemTest)
{
  FreqItem eItem({'e', 0xFF000000});
  FreqItem fItem({'f', 0x01000000});
  HaffmanEncoder encoder;
  VecByte expected = {
    0x65, 0x00, 0x00, 0x00, 0xFF
  };
  VecByte fbuffer, ebuffer;

  encoder.encode(fItem, fbuffer);
  encoder.encode(eItem, ebuffer);
  EXPECT_EQ(expected, ebuffer);
}

TEST_F(FrequencyTest, TestEncoding)
{
/*  VecByte encShouldBe = {
//     1     2     3     4     5     6     7     8     9    10    11    12    13    14    15    16
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0-15
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //16-31
    0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //32-47
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //48-63
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //64-79
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //80-95
    0x00, 0x00, 0x03, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, //96-111
    0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //112-127
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //128-143
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //144-159
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //160-175
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //176-191
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //192-207
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //208-223
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //224-239
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //240-255
    0x00, //padding
    0x29, 0xf3, 0x67, 0xca, 0x58
  };

  HaffmanEncoder encoder;
  std::string seq("beep boop beer!");
  VecByte encActual;
  encoder.encode(seq.begin(), seq.end(), encActual.begin());*/

}