#ifndef __SEPARATOR_TESTS__
#define __SEPARATOR_TESTS__

#include "../header/CRunMode.h"
#include "../header/CBase.h"
#include "../header/CCommand.h"
#include "../header/CSeparatorConnector.h"

TEST(SeparatorOpTestSet, TwoTrueCmd) {

    CCommand * pCmd1 = new CCommand ("echo first");
    CCommand * pCmd2 = new CCommand ("echo second");

    CSeparatorConnector exe1(pCmd1, pCmd2);

    EXPECT_EQ(exe1.execute(), true);
}

TEST(SeparatorOpTestSet, TwoFalseCmd) {

    CCommand * pCmd1 = new CCommand ("first");
    CCommand * pCmd2 = new CCommand ("second");

    CSeparatorConnector exe1(pCmd1, pCmd2);

    EXPECT_EQ(exe1.execute(), false);
}

// = + -
// = + -
// = + -

// = + -
// = + -
// = + -
// = + -
// = + -
// = + -

// = + -
// = + -
// = + -
// = + -
// = + -
// = + -
// = + -
// = + -
// = + -
// = + -
// = + -

// = + -
// = + -
// = + -
// = + -
// = + -
// = + -
// = + -
// = + -
// = + -
// = + -
// = + -
// = + -
// = + -
// = + -
// = + -
// = + -
// = + -
// = + -
// = + -
// = + -

// = + -
// = + -
// = + -
// = + -
// = + -
// = + -
/* = 2 1
23124
fb
fdgb
fg
gdf

-=
-=
-=-=-='-='
-=
-=

-=
=
=
-=
-
=
-=
-
=
-
=-
=
-
=
-=
-
=
-



#endif