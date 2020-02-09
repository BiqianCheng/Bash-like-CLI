#ifndef __RDIR_TESTS__
#define __RDIR_TESTS__

#include "../header/CRunMode.h"
#include "../header/CBase.h"
#include "../header/CCommand.h"
#include "../header/CRdirConnector.h"

TEST(RdirOpTestSet, RdirOutCmd) {

    CCommand * pCmd1 = new CCommand ("ls");
    CCommand * pCmd2 = new CCommand ("1.txt");

    CRdirOutConnector exe1(pCmd1, pCmd2);

    EXPECT_EQ(exe1.execute(),true);
}

TEST(RdirOpTestSet, RdirInCmd) {

    CCommand * pCmd1 = new CCommand ("tr a-z A-Z");
    CCommand * pCmd2 = new CCommand ("1.txt");

    CRdirInConnector exe1(pCmd1, pCmd2);

    EXPECT_EQ(exe1.execute(),true);
}


#endif