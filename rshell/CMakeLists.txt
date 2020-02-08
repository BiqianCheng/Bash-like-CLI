cmake_minimum_required(VERSION 3.15)
project(cs100proj)

set(CMAKE_CXX_STANDARD 11)

add_executable(rshell main.cpp CBase.h CConnector.h CAndConnector.h CCommand.h COrConnector.h CSeparatorConnector.h CRdirConnector.h CPipeConnector.h)