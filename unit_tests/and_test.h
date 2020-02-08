#ifndef __AND_TESTS__
#define __AND_TESTS__

#include "../header/CBase.h"
#include "../header/CAndConnector.h"
#include "../header/CCommand.h"
#include "../header/COrConnector.h"
#include "../header/CSeparatorConnector.h"

TEST(AndOpTestSet, TwoTrueCmd) {
  CCommand mock1("echo first");
  CCommand mock2("echo second");

  CAndConnector exe1(&mock1, &mock2);

  EXPECT_EQ(exe1.execute(), true);
}

TEST(AndOpTestSet, TwoFalseCmd) {
  CCommand mock1("aaa first");
  CCommand mock2("bbb second");

  CAndConnector exe1(&mock1, &mock2);

  EXPECT_EQ(exe1.execute(), false);
}


#endif