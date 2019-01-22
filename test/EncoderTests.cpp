//----------------------------------------------------------------------------------------------------------------------
#include "gtest/gtest.h"
//----------------------------------------------------------------------------------------------------------------------
#include "HaffmanEncoderImpl.h"
//----------------------------------------------------------------------------------------------------------------------
using namespace HaffmanImpl;
//----------------------------------------------------------------------------------------------------------------------
class EncodingTests : public testing::Test {
protected:
  virtual void SetUp() {
  };

  virtual void TearDown() {
  };

  HaffmanImpl::FrequencyTable _frequencyTable;
  HaffmanImpl::HaffmanEncoderImpl _encoder;
};
//----------------------------------------------------------------------------------------------------------------------
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
//----------------------------------------------------------------------------------------------------------------------
TEST_F(EncodingTests, SymFreqInitTest) {
  HaffmanImpl::LeafNode sf;
  EXPECT_EQ(0, sf.getFreq());
  EXPECT_EQ(0, sf.getFreq());
}
//----------------------------------------------------------------------------------------------------------------------
TEST_F(EncodingTests, TreeCodeInitTest) {
  TreeCode treeCode;
  EXPECT_EQ(0, treeCode._base);
  EXPECT_EQ(0, treeCode._size);
}
//----------------------------------------------------------------------------------------------------------------------
TEST_F(EncodingTests, SymFreqLessTest) {
  std::vector<HaffmanImpl::LeafNode> vec = {{'d', 5}, {'c', 1}, {'b', 1}};
  std::sort(vec.begin(), vec.end(),
            [](const HaffmanImpl::LeafNode & left, const HaffmanImpl::LeafNode & right) -> bool {
              return left.getFreq() > right.getFreq();
            });
  EXPECT_EQ('d', vec[0].getSym());
  EXPECT_EQ('c', vec[1].getSym());
  EXPECT_EQ('b', vec[2].getSym());
}
//----------------------------------------------------------------------------------------------------------------------
TEST_F(EncodingTests, EmptyDataTest) {}
//----------------------------------------------------------------------------------------------------------------------
TEST_F(EncodingTests, EncodingTests_Test) {
  const std::string seq1 = "beep b";
  const std::string seq2 = "oop beer!";
  _frequencyTable.takeFrequency(seq1.cbegin(), seq1.cend());
  _frequencyTable.takeFrequency(seq2.cbegin(), seq2.cend());
  _frequencyTable.buildTree();
  const HaffmanTree & haffmanTree = _frequencyTable.getHaffmanTree();

  ASSERT_NE(nullptr, haffmanTree.getTop());
  EXPECT_EQ(TreeNode::Type::Join, haffmanTree.getTop()->getType());
  EXPECT_EQ(15, haffmanTree.getTop()->getFreq());

  JoinNode * p0 = dynamic_cast<JoinNode *>(haffmanTree.getTop()->getLeft());
  ASSERT_NE(nullptr, p0);
  EXPECT_EQ(TreeNode::Type::Join, p0->getType());
  EXPECT_EQ(7, p0->getFreq());

  LeafNode * p00 = dynamic_cast<LeafNode *>(p0->getLeft());
  ASSERT_NE(nullptr, p00);
  EXPECT_EQ(TreeNode::Type::Leaf, p00->getType());
  EXPECT_EQ('b', p00->getSym());

  JoinNode * p01 = dynamic_cast<JoinNode *>(p0->getRight());
  ASSERT_NE(nullptr, p01);
  EXPECT_EQ(TreeNode::Type::Join, p01->getType());

  LeafNode * p011 = dynamic_cast<LeafNode *>(p01->getRight());
  ASSERT_NE(nullptr, p011);
  EXPECT_EQ(TreeNode::Type::Leaf, p011->getType());
  EXPECT_EQ('p', p011->getSym());

  JoinNode * p010 = dynamic_cast<JoinNode *>(p01->getLeft());
  ASSERT_NE(nullptr, p010);

  LeafNode * p0100 = dynamic_cast<LeafNode *>(p010->getLeft());
  ASSERT_NE(nullptr, p0100);
  EXPECT_EQ(TreeNode::Type::Leaf, p0100->getType());
  EXPECT_EQ('r', p0100->getSym());

  LeafNode * p0101 = dynamic_cast<LeafNode *>(p010->getRight());
  ASSERT_NE(nullptr, p0101);
  EXPECT_EQ(TreeNode::Type::Leaf, p0101->getType());
  EXPECT_EQ('!', p0101->getSym());

  JoinNode * p1 = dynamic_cast<JoinNode *>(haffmanTree.getTop()->getRight());
  ASSERT_NE(nullptr, p1);

  LeafNode * p10 = dynamic_cast<LeafNode *>(p1->getLeft());
  ASSERT_NE(nullptr, p10);
  EXPECT_EQ(TreeNode::Type::Leaf, p10->getType());
  EXPECT_EQ('e', p10->getSym());

  JoinNode * p11 = dynamic_cast<JoinNode *>(p1->getRight());
  ASSERT_NE(nullptr, p11);
  
  LeafNode * p111 = dynamic_cast<LeafNode *>(p11->getRight());
  ASSERT_NE(nullptr, p111);
  EXPECT_EQ(TreeNode::Type::Leaf, p111->getType());
  EXPECT_EQ('o', p111->getSym());

  LeafNode * p110 = dynamic_cast<LeafNode *>(p11->getLeft());
  ASSERT_NE(nullptr, p110);
  EXPECT_EQ(TreeNode::Type::Leaf, p110->getType());
  EXPECT_EQ(' ', p110->getSym());
}
//----------------------------------------------------------------------------------------------------------------------
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
//----------------------------------------------------------------------------------------------------------------------
TEST_F(EncodingTests, EncodingTests_TreeReadingPossibleAfterReseting_Test) {
  const std::string seq = "beep boop beer!";
  _frequencyTable.takeFrequency(seq.cbegin(), seq.cend());
  _frequencyTable.buildTree();
  const HaffmanTree & haffmanTree = _frequencyTable.getHaffmanTree();
  EXPECT_NE(nullptr, haffmanTree.getTop());

  EXPECT_EQ(TreeCode({0, 0}), haffmanTree.getCode('z'));
  EXPECT_EQ(TreeCode({0, 2}), haffmanTree.getCode('b'));
  EXPECT_EQ(TreeCode({2, 2}), haffmanTree.getCode('e'));
  EXPECT_EQ(TreeCode({3, 3}), haffmanTree.getCode('p'));
  EXPECT_EQ(TreeCode({6, 3}), haffmanTree.getCode(' '));
  EXPECT_EQ(TreeCode({7, 3}), haffmanTree.getCode('o'));

  const std::string seq2 = "mama mila ramu";
  _frequencyTable.reset();
  _frequencyTable.takeFrequency(seq2.cbegin(), seq2.cend());
  _frequencyTable.buildTree();
  const HaffmanTree & haffmanTree1 = _frequencyTable.getHaffmanTree();
  ASSERT_NE(nullptr, haffmanTree1.getTop());
  EXPECT_EQ(TreeNode::Type ::Join, haffmanTree1.getTop()->getType());
  EXPECT_EQ(14, haffmanTree1.getTop()->getFreq());

}
//----------------------------------------------------------------------------------------------------------------------
TEST_F(EncodingTests, EncodeFreqItemTest)
{
  VecByte buffer;
  FreqItem eItem({'e', 0xFF000000});
  _encoder.encode(eItem, buffer);

  VecByte expected = {
    0x65, 0x00, 0x00, 0x00, 0xFF
  };
  EXPECT_EQ(expected, buffer);
}
//----------------------------------------------------------------------------------------------------------------------
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

  WriteTreeCodeState treeCodeBuff;

  ushort toBeWrote = 0;
  ASSERT_EQ(false, treeCodeBuff.putAndCheckPossibleToWrite(b, toBeWrote));
  EXPECT_EQ(0, toBeWrote);
  EXPECT_EQ(0x00, treeCodeBuff._buffer);
  EXPECT_EQ(2, treeCodeBuff._bufferedCount);
  ASSERT_EQ(false, treeCodeBuff.putAndCheckPossibleToWrite(e, toBeWrote));
  EXPECT_EQ(0, toBeWrote);
  EXPECT_EQ(0x2000, treeCodeBuff._buffer);
  EXPECT_EQ(4, treeCodeBuff._bufferedCount);
  ASSERT_EQ(false, treeCodeBuff.putAndCheckPossibleToWrite(e, toBeWrote));
  EXPECT_EQ(0, toBeWrote);
  EXPECT_EQ(0x2800, treeCodeBuff._buffer);
  EXPECT_EQ(6, treeCodeBuff._bufferedCount);
  ASSERT_EQ(false, treeCodeBuff.putAndCheckPossibleToWrite(p, toBeWrote));
  EXPECT_EQ(0, toBeWrote);
  EXPECT_EQ(0x2980, treeCodeBuff._buffer);
  toBeWrote = 0;


  ASSERT_EQ(false, treeCodeBuff.putAndCheckPossibleToWrite(space, toBeWrote));
  EXPECT_EQ(0x00, toBeWrote);
  EXPECT_EQ(0x29E0, treeCodeBuff._buffer);
  EXPECT_EQ(12, treeCodeBuff._bufferedCount);
  ASSERT_EQ(false, treeCodeBuff.putAndCheckPossibleToWrite(b, toBeWrote));
  EXPECT_EQ(0x00, toBeWrote);
  EXPECT_EQ(0x29E0, treeCodeBuff._buffer);
  EXPECT_EQ(14, treeCodeBuff._bufferedCount);
  ASSERT_EQ(true, treeCodeBuff.putAndCheckPossibleToWrite(o, toBeWrote));
  EXPECT_EQ(0x29E3, toBeWrote);
  EXPECT_EQ(0x8000, treeCodeBuff._buffer);
  EXPECT_EQ(1, treeCodeBuff._bufferedCount);
  toBeWrote = 0;

  ASSERT_EQ(false, treeCodeBuff.putAndCheckPossibleToWrite(o, toBeWrote));
  EXPECT_EQ(0x000, toBeWrote);
  EXPECT_EQ(0xF000, treeCodeBuff._buffer);
  EXPECT_EQ(4, treeCodeBuff._bufferedCount);

  ASSERT_EQ(false, treeCodeBuff.putAndCheckPossibleToWrite(p, toBeWrote));
  EXPECT_EQ(0x0000, toBeWrote);
  EXPECT_EQ(0xF600, treeCodeBuff._buffer);
  EXPECT_EQ(7, treeCodeBuff._bufferedCount);

  ASSERT_EQ(false, treeCodeBuff.putAndCheckPossibleToWrite(space, toBeWrote));
  EXPECT_EQ(0, toBeWrote);
  EXPECT_EQ(0xF780, treeCodeBuff._buffer);
  EXPECT_EQ(10, treeCodeBuff._bufferedCount);
  toBeWrote = 0;

  ASSERT_EQ(false, treeCodeBuff.putAndCheckPossibleToWrite(b, toBeWrote));
  EXPECT_EQ(0x0000, toBeWrote);
  EXPECT_EQ(0xF780, treeCodeBuff._buffer);
  EXPECT_EQ(12, treeCodeBuff._bufferedCount);

  ASSERT_EQ(false, treeCodeBuff.putAndCheckPossibleToWrite(e, toBeWrote));
  EXPECT_EQ(0x00, toBeWrote);
  EXPECT_EQ(0xF788, treeCodeBuff._buffer);
  EXPECT_EQ(14, treeCodeBuff._bufferedCount);

  ASSERT_EQ(true, treeCodeBuff.putAndCheckPossibleToWrite(e, toBeWrote));
  EXPECT_EQ(0xF78A, toBeWrote);
  EXPECT_EQ(0, treeCodeBuff._buffer);
  EXPECT_EQ(0, treeCodeBuff._bufferedCount);
  toBeWrote = 0;

  ASSERT_EQ(false, treeCodeBuff.putAndCheckPossibleToWrite(r, toBeWrote));
  EXPECT_EQ(0x00, toBeWrote);
  EXPECT_EQ(0x4000, treeCodeBuff._buffer);
  EXPECT_EQ(4, treeCodeBuff._bufferedCount);

  ASSERT_EQ(false, treeCodeBuff.putAndCheckPossibleToWrite(shout, toBeWrote));
  EXPECT_EQ(0x00, toBeWrote);
  EXPECT_EQ(0x4500, treeCodeBuff._buffer);
  EXPECT_EQ(8, treeCodeBuff._bufferedCount);
  toBeWrote = 0;

  VecTreeCode vecTreeCode { b, e, e, p, space,  b, o, o, p, space, b, e, e, r, shout };

  VecByte expected {
    0x0F, 0x00, 0x00, 0x00,        // size
    0xE3, 0x29, 0x8A, 0xF7, 0x00, 0x45   // payload
  };
  VecByte buffer;
  ASSERT_TRUE(_encoder.encodePayload(str.begin(), str.end(), buffer));
  EXPECT_EQ(buffer, expected);
}
//----------------------------------------------------------------------------------------------------------------------
TEST_F(EncodingTests, TestEncoding)
{
  VecByte expected = {
    0x07, 0x00,                         //size of frequency pack
    ' ', 0x02, 0x00, 0x00, 0x00,
    '!', 0x01, 0x00, 0x00, 0x00,
    'b', 0x03, 0x00, 0x00, 0x00,
    'e', 0x04, 0x00, 0x00, 0x00,
    'o', 0x02, 0x00, 0x00, 0x00,
    'p', 0x02, 0x00, 0x00, 0x00,
    'r', 0x01, 0x00, 0x00, 0x00,
    0x0F, 0x00, 0x00, 0x00,        // size
    0xE3, 0x29, 0x8A, 0xF7, 0x00, 0x45   // payload
  };

  HaffmanEncoderImpl encoder;
  std::string seq("beep boop beer!");
  VecByte buffer;
  ASSERT_TRUE(encoder.encodeBlock(seq.begin(), seq.end(), buffer));
  EXPECT_EQ(expected, buffer);

  VecByte expectedTwoEmpty = {
    0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
  };
  buffer.clear();
  std::string empty;
  ASSERT_TRUE(encoder.encodeBlock(empty.begin(), empty.end(), buffer));
  ASSERT_TRUE(encoder.encodeBlock(empty.begin(), empty.end(), buffer));
  EXPECT_EQ(expectedTwoEmpty, buffer);
}
//----------------------------------------------------------------------------------------------------------------------
TEST_F(EncodingTests, CanUseEncoderTwiceTest)
{
  std::string str = "beep boop beer!";
  std::string stuff = "adsfasdfjlkjadsflkj";
  HaffmanEncoderImpl singleUsedEncoder;
  HaffmanEncoderImpl encoder;

  VecByte encFirs, encSecond, encThird;
  encoder.encodeBlock(stuff.begin(), stuff.end(), encFirs);
  encoder.encodeBlock(str.begin(), str.end(), encSecond);
  encoder.encodeBlock(str.begin(), str.end(), encThird);
  EXPECT_EQ(encThird, encSecond);
}
//----------------------------------------------------------------------------------------------------------------------
