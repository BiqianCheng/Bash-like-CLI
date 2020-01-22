CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
project(rshell)

ADD_SUBDIRECTORY(googletest)


set(HEADERS header/base.h)
set(SOURCES src/main.cpp src/base.cpp)
set(TESTING unit_tests/test_main.cpp)

ADD_EXECUTABLE(rshell
   ${SOURCES}
)

ADD_EXECUTABLE(test
   ${HEADERS} ${TESTING}
)

TARGET_LINK_LIBRARIES(test gtest)
TARGET_COMPILE_DEFINITIONS(test PRIVATE gtest_disable_pthreads=ON)
TARGET_COMPILE_DEFINITIONS(test PRIVATE gtest_disable_pthreads=ON)
