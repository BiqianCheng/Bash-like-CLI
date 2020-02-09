#ifndef __RDIR_TESTS__
#define __RDIR_TESTS__

#include "../header/CRunMode.h"
#include "../header/CBase.h"
#include "../header/CCommand.h"
#include "../header/CRdirConnector.h"

TEST(RdirOpTestSet, TwoTrueCmd) {

    CCommand mock1("echo first");
    CCommand mock2("echo second");

    CRdirConnector exe1(&mock1, &mock2);

    EXPECT_EQ(exe1.execute(),true);
}

TEST(RdirOpTestSet, TwoFalseCmd) {

    CCommand mock1("first");
    CCommand mock2("second");

    CRdirConnector exe1(&mock1, &mock2);

    EXPECT_EQ(exe1.execute(),false);
}


#endif