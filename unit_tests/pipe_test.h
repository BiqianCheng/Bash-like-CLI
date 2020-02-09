#ifndef __PIPE_TESTS__
#define __PIPE_TESTS__

#include "../header/CRunMode.h"
#include "../header/CBase.h"
#include "../header/CCommand.h"
#include "../header/CPipeConnector.h"


TEST(PipeOpTestSet, TwoTrueCmd) {

    CCommand mock1("ls -l");
    CCommand mock2("wc -l");

    CPipeConnector exe1(&mock1, &mock2);

    EXPECT_EQ(exe1.execute(),false);
}




#endif