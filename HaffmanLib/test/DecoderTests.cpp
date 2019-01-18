#include <HaffmanEncoderImpl.h>
#include "gtest/gtest.h"
#include "HaffmanDecoderImpl.h"

using namespace Haffman;

class DecodingTests : public testing::Test {
protected:
  virtual void SetUp() {
  };

  virtual void TearDown() {
  };

  Haffman::HaffmanDecoderImpl _decoder;
};

TEST_F(DecodingTests, DecodeEmptyTest)
{

}

TEST_F(DecodingTests, DecodeFrequencyTableTest)
{
  VecByte data1 = {
    0x07,                         //size of frequency pack
    ' ', 0x02, 0x00, 0x00, 0x00,
    '!', 0x01, 0x00, 0x00, 0x00,
    'b', 0x03, 0x00, 0x00, 0x00,
    'e', 0x04
  };

  VecByte data2 = {
    0x00, 0x00, 0x00,
    'o', 0x02, 0x00, 0x00, 0x00,
    'p', 0x02, 0x00, 0x00, 0x00,
    'r', 0x01, 0x00, 0x00, 0x00,
  };

  _decoder.addData(data1.begin(), data1.end());
  ASSERT_FALSE(_decoder.decodeHead());
  _decoder.addData(data2.begin(), data2.end());
  ASSERT_TRUE(_decoder.decodeHead());

  const FrequencyTable & freqTable = _decoder.getFrequencyTable();
  EXPECT_EQ(0, freqTable.getFrequencyOf('8'));
  EXPECT_EQ(2, freqTable.getFrequencyOf(' '));
  EXPECT_EQ(1, freqTable.getFrequencyOf('!'));
  EXPECT_EQ(3, freqTable.getFrequencyOf('b'));
  EXPECT_EQ(4, freqTable.getFrequencyOf('e'));
  EXPECT_EQ(2, freqTable.getFrequencyOf('o'));
  EXPECT_EQ(2, freqTable.getFrequencyOf('p'));
  EXPECT_EQ(1, freqTable.getFrequencyOf('r'));
  HaffmanTree haffmanTree = _decoder.getFrequencyTable().getHaffmanTree();

  EXPECT_EQ(TreeCode({0, 0}), haffmanTree.getCode('v'));
  EXPECT_EQ(TreeCode({4, 4}), haffmanTree.getCode('r'));
  EXPECT_EQ(TreeCode({5, 4}), haffmanTree.getCode('!'));
  EXPECT_EQ(TreeCode({0, 2}), haffmanTree.getCode('b'));
  EXPECT_EQ(TreeCode({6, 3}), haffmanTree.getCode(' '));
  EXPECT_EQ(TreeCode({7, 3}), haffmanTree.getCode('o'));
  EXPECT_EQ(TreeCode({3, 3}), haffmanTree.getCode('p'));
  EXPECT_EQ(TreeCode({2, 2}), haffmanTree.getCode('e'));
}

TEST_F(DecodingTests, DecodeSequence)
{

  VecByte freqTableData = {
    0x07,                         //size of frequency pack
    ' ', 0x02, 0x00, 0x00, 0x00,
    '!', 0x01, 0x00, 0x00, 0x00,
    'b', 0x03, 0x00, 0x00, 0x00,
    'e', 0x04, 0x00, 0x00, 0x00,
    'o', 0x02, 0x00, 0x00, 0x00,
    'p', 0x02, 0x00, 0x00, 0x00,
    'r', 0x01, 0x00, 0x00, 0x00,
  };

  _decoder.addData(freqTableData.begin(), freqTableData.end());
  ASSERT_TRUE(_decoder.decodeHead());

  VecByte encPayload = {
    0x0F, 0x00, 0x00, 0x00,       // encBytesCount
    0x29, 0xE3, 0xF7, 0x8A, 0x45, // payload
    0x00, 0x00, 0xFE, 0xAA        // trash
  };
  _decoder.addData(encPayload.begin(), encPayload.end());
  VecByte buffer;
  _decoder.decodePayload(buffer);


  VecByte expected {'b', 'e', 'e', 'p', ' ', 'b', 'o', 'o', 'p', ' ', 'b', 'e', 'e', 'r', '!'};
  EXPECT_EQ(expected, buffer);
}

TEST_F(DecodingTests, DecodeInTwoStepsTest)
{

  HaffmanEncoderImpl encoder;
  std::string str = "beep boop beer!beep boop beer!\55";
  VecByte encBlocksCountBlock = {0x02, 0x00, 0x00, 0x00 };

  VecByte encBuffer;
  ASSERT_TRUE(encoder.encodeBlock(str.begin(), str.begin() + 30, encBuffer));
  ASSERT_TRUE(encoder.encodeBlock(str.begin() + 30, str.end(), encBuffer));

  VecByte decBuffer;
  EXPECT_FALSE(_decoder.addDataAndTryToDecode(encBlocksCountBlock.begin(), encBlocksCountBlock.end(), decBuffer));
  EXPECT_TRUE(_decoder.addDataAndTryToDecode(encBuffer.begin(), encBuffer.end(), decBuffer));

  std::string result(decBuffer.begin(), decBuffer.end());
  EXPECT_EQ(VecByte(str.begin(), str.end()), decBuffer);
}

