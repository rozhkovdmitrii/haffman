#include "gtest/gtest.h"
#include "Log.h"
#include "FrequencyTable.h"

int main(int argc, char *argv[]) {
  Log::setEnabled(Log::Type::DBGERR, false);
  Log::setEnabled(Log::Type::APPERR, true);
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}