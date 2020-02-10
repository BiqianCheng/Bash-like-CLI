#include "gtest/gtest.h"

#include "and_test.h"
#include "or_test.h"
#include "separator_test.h"

#include "rdir_test.h"
#include "pipe_test.h"


CRunMode::RunningMode  CRunMode::nMode = CRunMode::R_TESTING;

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
