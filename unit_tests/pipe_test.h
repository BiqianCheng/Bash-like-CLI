#ifndef __PIPE_TESTS__
#define __PIPE_TESTS__

#include "../header/CRunMode.h"
#include "../header/CBase.h"
#include "../header/CCommand.h"
#include "../header/CPipeConnector.h"


TEST(PipeOpTestSet, TwoTrueCmd) {

    CCommand mock1("echo first");
    CCommand mock2("echo second");

    CPipeConnector exe1(&mock1, &mock2);

    EXPECT_EQ(exe1.execute(),true);
}

TEST(PipeOpTestSet, TwoFalseCmd) {

    CCommand mock1("first");
    CCommand mock2("second");

    CPipeConnector exe1(&mock1, &mock2);

    EXPECT_EQ(exe1.execute(),false);
}


#endif