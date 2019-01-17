#include "gtest/gtest.h"
#include "HaffmanDecoder.h"

using namespace Haffman;

class DecodingTests : public testing::Test {
protected:
  virtual void SetUp() {
  };

  virtual void TearDown() {
  };

  Haffman::HaffmanDecoder _decoder;
};

TEST_F(DecodingTests, DecodeEmptyTest)
{

}

TEST_F(DecodingTests, DecodeFrequencyTableTest)
{
  VecByte data = {
    0x07,                         //size of frequency pack
    ' ', 0x02, 0x00, 0x00, 0x00,
    '!', 0x01, 0x00, 0x00, 0x00,
    'b', 0x03, 0x00, 0x00, 0x00,
    'e', 0x04, 0x00, 0x00, 0x00,
    'o', 0x02, 0x00, 0x00, 0x00,
    'p', 0x02, 0x00, 0x00, 0x00,
    'r', 0x01, 0x00, 0x00, 0x00,
  };
  FrequencyTable freqTable;
  _decoder.addData(data);
  ASSERT_TRUE(_decoder.decodeFrequencyTable(freqTable));
  EXPECT_EQ(0, freqTable.getFrequencyOf('8'));
  EXPECT_EQ(2, freqTable.getFrequencyOf(' '));
  EXPECT_EQ(1, freqTable.getFrequencyOf('!'));
  EXPECT_EQ(3, freqTable.getFrequencyOf('b'));
  EXPECT_EQ(4, freqTable.getFrequencyOf('e'));
  EXPECT_EQ(2, freqTable.getFrequencyOf('o'));
  EXPECT_EQ(2, freqTable.getFrequencyOf('p'));
  EXPECT_EQ(1, freqTable.getFrequencyOf('r'));
}