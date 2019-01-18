#include "gtest/gtest.h"
#include "ErrLog.h"
#include "FrequencyTable.h"

int main(int argc, char *argv[]) {
  ErrLog::disable();
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}