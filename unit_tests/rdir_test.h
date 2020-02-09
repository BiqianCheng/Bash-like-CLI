#ifndef __RDIR_TESTS__
#define __RDIR_TESTS__

#include "../header/CRunMode.h"
#include "../header/CBase.h"
#include "../header/CCommand.h"
#include "../header/CRdirConnector.h"

TEST(RdirOpTestSet, RdirOutCmd) {

    CCommand mock1("ls");
    CCommand mock2("1.txt");

    CRdirOutConnector exe1(&mock1, &mock2);

    EXPECT_EQ(exe1.execute(),true);
}

TEST(RdirOpTestSet, RdirInCmd) {

    CCommand mock1("tr a-z A-Z");
    CCommand mock2("1.txt");

    CRdirInConnector exe1(&mock1, &mock2);

    EXPECT_EQ(exe1.execute(),false);
}


#endif