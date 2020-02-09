#ifndef __OR_TESTS__
#define __OR_TESTS__

#include "../header/CRunMode.h"
#include "../header/CBase.h"
#include "../header/CCommand.h"
#include "../header/COrConnector.h"


TEST(OrOpTestSet, TwoTrueCmd) {

    CCommand mock1("echo first");
    CCommand mock2("echo second");

    COrConnector exe1(&mock1, &mock2);

    EXPECT_EQ(exe1.execute(), true);
}

TEST(OrOpTestSet, TwoFalseCmd) {

    CCommand mock1("first");
    CCommand mock2("second");

    COrConnector exe1(&mock1, &mock2);

    EXPECT_EQ(exe1.execute(), false);
}


#endif