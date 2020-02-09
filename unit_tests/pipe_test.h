#ifndef __PIPE_TESTS__
#define __PIPE_TESTS__

#include "../header/CRunMode.h"
#include "../header/CBase.h"
#include "../header/CCommand.h"
#include "../header/CPipeConnector.h"


TEST(PipeOpTestSet, TwoTrueCmd) {

    CCommand * pCmd1 = new CCommand ("ls -l");
    CCommand * pCmd2 = new CCommand ("wc -l");

    CPipeConnector exe1(pCmd1, pCmd2);

    EXPECT_EQ(exe1.execute(),true);
}




#endif