#include "gtest/gtest.h"
#include "ErrLog.h"
#include "FrequencyTable.h"

int main(int argc, char *argv[]) {
  ErrLog::setEnabled(ErrLog::Type::DBGERR, false);
  ErrLog::setEnabled(ErrLog::Type::APPERR, true);
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}