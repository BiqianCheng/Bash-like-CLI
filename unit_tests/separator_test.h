#ifndef __SEPARATOR_TESTS__
#define __SEPARATOR_TESTS__

#include "../header/CRunMode.h"
#include "../header/CBase.h"
#include "../header/CCommand.h"
#include "../header/CSeparatorConnector.h"

TEST(SeparatorOpTestSet, TwoTrueCmd) {

    CCommand mock1("echo first");
    CCommand mock2("echo second");

    CSeparatorConnector exe1(&mock1, &mock2);

    EXPECT_EQ(exe1.execute(), true);
}

TEST(SeparatorOpTestSet, TwoFalseCmd) {

    CCommand mock1("first");
    CCommand mock2("second");

    CSeparatorConnector exe1(&mock1, &mock2);

    EXPECT_EQ(exe1.execute(), false);
}


#endif