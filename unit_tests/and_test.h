#ifndef __AND_TESTS__
#define __AND_TESTS__

#include "../header/CRunMode.h"
#include "../header/CBase.h"
#include "../header/CCommand.h"
#include "../header/CAndConnector.h"


TEST(AndOpTestSet, TwoTrueCmd) {

    CCommand mock1("echo first");
    CCommand mock2("echo second");

    CAndConnector exe1(&mock1, &mock2);

    EXPECT_EQ(exe1.execute(), true);
}

TEST(AndOpTestSet, TwoFalseCmd) {

    CCommand mock1("first");
    CCommand mock2("second");

    CAndConnector exe1(&mock1, &mock2);

    EXPECT_EQ(exe1.execute(), false);
}


#endif