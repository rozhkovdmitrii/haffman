#include "gtest/gtest.h"
#include "HaffmanEncoder.h"

using namespace Haffman;

class EncodingTests : public testing::Test {
protected:
  virtual void SetUp() {
  };

  virtual void TearDown() {
  };

  Haffman::FrequencyTable _frequencyTable;
  Haffman::HaffmanEncoder _encoder;
};

TEST_F(EncodingTests, EncodingTests_TestReadingSimple_Test) {
  const std::string seq = "beep boop beer!";
  _frequencyTable.takeFrequency((byte *) seq.data(), ((byte *) seq.data()) + seq.size());
  EXPECT_EQ(4, _frequencyTable.getFrequencyOf('e'));
  EXPECT_EQ(3, _frequencyTable.getFrequencyOf('b'));
  EXPECT_EQ(2, _frequencyTable.getFrequencyOf('p'));
  EXPECT_EQ(2, _frequencyTable.getFrequencyOf(' '));
  EXPECT_EQ(1, _frequencyTable.getFrequencyOf('r'));
  EXPECT_EQ(1, _frequencyTable.getFrequencyOf('!'));
  EXPECT_EQ(2, _frequencyTable.getFrequencyOf('o'));
}

TEST_F(EncodingTests, SymFreqInitTest) {
  Haffman::LeafNode sf;
  EXPECT_EQ(0, sf.getFreq());
  EXPECT_EQ(0, sf.getFreq());
}

TEST_F(EncodingTests, TreeCodeInitTest) {
  TreeCode treeCode;
  EXPECT_EQ(0, treeCode._base);
  EXPECT_EQ(0, treeCode._size);
}

TEST_F(EncodingTests, SymFreqLessTest) {
  std::vector<Haffman::LeafNode> vec = {{'d', 5}, {'c', 1}, {'b', 1}};
  std::sort(vec.begin(), vec.end(),
            [](const Haffman::LeafNode & left, const Haffman::LeafNode & right) -> bool {
              return left.getFreq() > right.getFreq();
            });
  EXPECT_EQ('d', vec[0]._sym);
  EXPECT_EQ('c', vec[1]._sym);
  EXPECT_EQ('b', vec[2]._sym);
}

TEST_F(EncodingTests, EmptyDataTest) {}

TEST_F(EncodingTests, EncodingTests_Test) {
  const std::string seq1 = "beep b";
  const std::string seq2 = "oop beer!";
  _frequencyTable.takeFrequency(seq1.cbegin(), seq1.cend());
  _frequencyTable.takeFrequency(seq2.cbegin(), seq2.cend());
  HaffmanTree haffmanTree = _frequencyTable.getHaffmanTree();

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

TEST_F(EncodingTests, FrequencyTable_TreeCodeTest_Test) {
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

TEST_F(EncodingTests, EncodingTests_TreeReadingPossibleAfterReseting_Test) {
  const std::string seq = "beep boop beer!";
  _frequencyTable.takeFrequency(seq.cbegin(), seq.cend());

  HaffmanTree haffmanTree = _frequencyTable.getHaffmanTree();
  EXPECT_NE(nullptr, haffmanTree.getTop());

  EXPECT_EQ(TreeCode({0, 0}), haffmanTree.getCode('z'));
  EXPECT_EQ(TreeCode({0, 2}), haffmanTree.getCode('b'));
  EXPECT_EQ(TreeCode({2, 2}), haffmanTree.getCode('e'));
  EXPECT_EQ(TreeCode({3, 3}), haffmanTree.getCode('p'));
  EXPECT_EQ(TreeCode({6, 3}), haffmanTree.getCode(' '));
  EXPECT_EQ(TreeCode({7, 3}), haffmanTree.getCode('o'));

  const std::string seq2 = "mama mila ramu";
  auto size = strlen(seq2.c_str());
  _frequencyTable.reset();
  _frequencyTable.takeFrequency(seq2.cbegin(), seq2.cend());

  HaffmanTree haffmanTree1 = _frequencyTable.getHaffmanTree();
  ASSERT_NE(nullptr, haffmanTree1.getTop());
  EXPECT_EQ(TreeNode::Type ::Join, haffmanTree1.getTop()->getType());
  EXPECT_EQ(14, haffmanTree1.getTop()->getFreq());

}

TEST_F(EncodingTests, EncodeFreqItemTest)
{
  FreqItem eItem({'e', 0xFF000000});
  FreqItem fItem({'f', 0x01000000});
  VecByte expected = {
    0x65, 0x00, 0x00, 0x00, 0xFF
  };
  VecByte fbuffer, ebuffer;

  _encoder.encode(fItem, fbuffer);
  _encoder.encode(eItem, ebuffer);
  EXPECT_EQ(expected, ebuffer);
}


TEST_F(EncodingTests, EncodingTreeCodesTest)
{
  std::string str = "beep boop beer!";
  _encoder.prepareToEncode(str.begin(), str.end());

  const HaffmanTree & haffmanTree = _encoder.getHaffmanTree();

  const TreeCode & space = haffmanTree.getCode(' ');
  const TreeCode & b = haffmanTree.getCode('b');
  const TreeCode & e = haffmanTree.getCode('e');
  const TreeCode & p = haffmanTree.getCode('p');
  const TreeCode & shout = haffmanTree.getCode('!');
  const TreeCode & o = haffmanTree.getCode('o');
  const TreeCode & r = haffmanTree.getCode('r');

  TreeCodeBuff treeCodeBuff;

  byte toBeWrote = 0;
  ASSERT_EQ(false, treeCodeBuff.emplace(b, toBeWrote));
  EXPECT_EQ(0, toBeWrote);
  EXPECT_EQ(0x00, treeCodeBuff._buffer);
  EXPECT_EQ(2, treeCodeBuff._bufferedCount);
  ASSERT_EQ(false, treeCodeBuff.emplace(e, toBeWrote));
  EXPECT_EQ(0, toBeWrote);
  EXPECT_EQ(0x20, treeCodeBuff._buffer);
  EXPECT_EQ(4, treeCodeBuff._bufferedCount);
  ASSERT_EQ(false, treeCodeBuff.emplace(e, toBeWrote));
  EXPECT_EQ(0, toBeWrote);
  EXPECT_EQ(0x28, treeCodeBuff._buffer);
  EXPECT_EQ(6, treeCodeBuff._bufferedCount);
  ASSERT_EQ(true, treeCodeBuff.emplace(p, toBeWrote));
  EXPECT_EQ(0x29, toBeWrote);
  EXPECT_EQ(0x80, treeCodeBuff._buffer);
  toBeWrote = 0;

  EXPECT_EQ(1, treeCodeBuff._bufferedCount);
  ASSERT_EQ(false, treeCodeBuff.emplace(space, toBeWrote));
  EXPECT_EQ(0x00, toBeWrote);
  EXPECT_EQ(0xE0, treeCodeBuff._buffer);
  EXPECT_EQ(4, treeCodeBuff._bufferedCount);
  ASSERT_EQ(false, treeCodeBuff.emplace(b, toBeWrote));
  EXPECT_EQ(0x00, toBeWrote);
  EXPECT_EQ(0xE0, treeCodeBuff._buffer);
  EXPECT_EQ(6, treeCodeBuff._bufferedCount);
  ASSERT_EQ(true, treeCodeBuff.emplace(o, toBeWrote));
  EXPECT_EQ(0xE3, toBeWrote);
  EXPECT_EQ(0x80, treeCodeBuff._buffer);
  EXPECT_EQ(1, treeCodeBuff._bufferedCount);
  toBeWrote = 0;

  ASSERT_EQ(false, treeCodeBuff.emplace(o, toBeWrote));
  EXPECT_EQ(0x00, toBeWrote);
  EXPECT_EQ(0xF0, treeCodeBuff._buffer);
  EXPECT_EQ(4, treeCodeBuff._bufferedCount);

  ASSERT_EQ(false, treeCodeBuff.emplace(p, toBeWrote));
  EXPECT_EQ(0x00, toBeWrote);
  EXPECT_EQ(0xF6, treeCodeBuff._buffer);
  EXPECT_EQ(7, treeCodeBuff._bufferedCount);

  ASSERT_EQ(true, treeCodeBuff.emplace(space, toBeWrote));
  EXPECT_EQ(0xF7, toBeWrote);
  EXPECT_EQ(0x80, treeCodeBuff._buffer);
  EXPECT_EQ(2, treeCodeBuff._bufferedCount);
  toBeWrote = 0;

  ASSERT_EQ(false, treeCodeBuff.emplace(b, toBeWrote));
  EXPECT_EQ(0x00, toBeWrote);
  EXPECT_EQ(0x80, treeCodeBuff._buffer);
  EXPECT_EQ(4, treeCodeBuff._bufferedCount);

  ASSERT_EQ(false, treeCodeBuff.emplace(e, toBeWrote));
  EXPECT_EQ(0x00, toBeWrote);
  EXPECT_EQ(0x88, treeCodeBuff._buffer);
  EXPECT_EQ(6, treeCodeBuff._bufferedCount);

  ASSERT_EQ(true, treeCodeBuff.emplace(e, toBeWrote));
  EXPECT_EQ(0x8A, toBeWrote);
  EXPECT_EQ(0x00, treeCodeBuff._buffer);
  EXPECT_EQ(0, treeCodeBuff._bufferedCount);
  toBeWrote = 0;

  ASSERT_EQ(false, treeCodeBuff.emplace(r, toBeWrote));
  EXPECT_EQ(0x00, toBeWrote);
  EXPECT_EQ(0x40, treeCodeBuff._buffer);
  EXPECT_EQ(4, treeCodeBuff._bufferedCount);

  ASSERT_EQ(true, treeCodeBuff.emplace(shout, toBeWrote));
  EXPECT_EQ(0x45, toBeWrote);
  EXPECT_EQ(0x00, treeCodeBuff._buffer);
  EXPECT_EQ(0, treeCodeBuff._bufferedCount);
  toBeWrote = 0;

  VecTreeCode vecTreeCode { b, e, e, p, space,  b, o, o, p, space, b, e, e, r, shout };

  VecByte expected {
    0x0A, 0x00, 0x00, 0x00,        // size
    0x00,                          // padding
    0x29, 0xE3, 0xF7, 0x8A, 0x45   // payload
  };
  VecByte buffer;
  ASSERT_TRUE(_encoder.encodePayload(str.begin(), str.end(), buffer));
  EXPECT_EQ(buffer, expected);
}

TEST_F(EncodingTests, TestEncoding)
{
  VecByte expected = {
    0x07,                         //size of frequency pack
    ' ', 0x02, 0x00, 0x00, 0x00,
    '!', 0x01, 0x00, 0x00, 0x00,
    'b', 0x03, 0x00, 0x00, 0x00,
    'e', 0x04, 0x00, 0x00, 0x00,
    'o', 0x02, 0x00, 0x00, 0x00,
    'p', 0x02, 0x00, 0x00, 0x00,
    'r', 0x01, 0x00, 0x00, 0x00,
    0x0A, 0x00, 0x00, 0x00,        // size
    0x00,                          // padding
    0x29, 0xE3, 0xF7, 0x8A, 0x45   // payload
  };

  HaffmanEncoder encoder;
  std::string seq("beep boop beer!");
  VecByte buffer;
  ASSERT_TRUE(encoder.encodeBlock(seq.begin(), seq.end(), buffer));
  EXPECT_EQ(expected, buffer);

  VecByte expectedTwoEmpty = {
    0x00,

    0x05, 0x00, 0x00, 0x00,
    0x00,
    0x00,

    0x05, 0x00, 0x00, 0x00,
    0x00
  };
  buffer.clear();
  std::string empty;
  ASSERT_TRUE(encoder.encodeBlock(empty.begin(), empty.end(), buffer));
  ASSERT_TRUE(encoder.encodeBlock(empty.begin(), empty.end(), buffer));
  EXPECT_EQ(expectedTwoEmpty, buffer);
}
