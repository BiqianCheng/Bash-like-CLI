#ifndef __OR_TESTS__
#define __OR_TESTS__

#include "../header/CRunMode.h"
#include "../header/CBase.h"
#include "../header/CCommand.h"
#include "../header/COrConnector.h"


TEST(OrOpTestSet, TwoTrueCmd) {

    CCommand * pCmd1 = new CCommand ("echo first");
    CCommand * pCmd2 = new CCommand ("echo second");

    COrConnector exe1(pCmd1, pCmd2);

    EXPECT_EQ(exe1.execute(), true);
}

TEST(OrOpTestSet, LeftFalseCmd) {

    CCommand * pCmd1 = new CCommand ("echo first");
    CCommand * pCmd2 = new CCommand ("echo second");

    COrConnector exe1(pCmd1, pCmd2);

    EXPECT_EQ(exe1.execute(), true);
}

TEST(OrOpTestSet, RightFalseCmd) {

    CCommand * pCmd1 = new CCommand ("echo first");
    CCommand * pCmd2 = new CCommand ("echo second");

    COrConnector exe1(pCmd1, pCmd2);

    EXPECT_EQ(exe1.execute(), true);
}



TEST(OrOpTestSet, TwoFalseCmd) {

    CCommand * pCmd1 = new CCommand ("first");
    CCommand * pCmd2 = new CCommand ("second");

    COrConnector exe1(pCmd1, pCmd2);

    EXPECT_EQ(exe1.execute(), false);
}


#endif