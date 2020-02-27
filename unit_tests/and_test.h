#ifndef __AND_TESTS__
#define __AND_TESTS__

#include "../header/CRunMode.h"
#include "../header/CBase.h"
#include "../header/CCommand.h"
#include "../header/CAndConnector.h"


TEST(AndOpTestSet, TwoTrueCmd) {

    CCommand * pCmd1 = new CCommand ("echo first");
    CCommand * pCmd2 = new CCommand ("echo second");

    CAndConnector exe1(pCmd1, pCmd2);

    EXPECT_EQ(exe1.execute(), true);
}

TEST(AndOpTestSet, TwoTrueCmdWithComment) {


    CCommand * pCmd1 = new CCommand ("echo first  #aaa");
    CCommand * pCmd2 = new CCommand ("echo second #bbb");

    CAndConnector exe1(pCmd1, pCmd2);

    EXPECT_EQ(exe1.execute(), true);
}

TEST(AndOpTestSet, TwoFalseCmd) {

    CCommand * pCmd1 = new CCommand ("ls ");
    CCommand * pCmd2 = new CCommand ("ls ");
    CAndConnector exe1(pCmd1, pCmd2);

    EXPECT_EQ(exe1.execute(), true);
}



TEST(AndOpTestSet, OneLeftFalseCmd) {


    CCommand * pCmd1 = new CCommand ("cmdA ");
    CCommand * pCmd2 = new CCommand ("ls");
    CAndConnector exe1(pCmd1, pCmd2);

    EXPECT_EQ(exe1.execute(), false);
}


TEST(AndOpTestSet, OneRightFalseCmd) {

    CCommand * pCmd1 = new CCommand ("ls ");
    CCommand * pCmd2 = new CCommand ("cmdB");

    CAndConnector exe1(pCmd1, pCmd2);

    EXPECT_EQ(exe1.execute(), false);
}


#endif