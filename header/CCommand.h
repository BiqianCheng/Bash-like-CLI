#ifndef CS100PROJ_CCOMMAND_H
#define CS100PROJ_CCOMMAND_H

#include "CBase.h"
#include "CRunMode.h"
#include "CParser.h"


#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>

#include <cstring>

#include <vector>
#include <string>
#include <fstream>


using namespace std;


class CCommand : public CBase {
public:
    vector<string> vecToken;
    vector<string> vecAnother;

    string strInData;
    string strOutData;

    string strInFileName;
    string strOutFileName;

    unsigned char advMode; // 0-none 1-in 2-out 3-in and out 4-pipe
    unsigned char skipIt;

    CCommand() ;

    CCommand(const char *pFullLine) ;
    CCommand(vector<string> & v ) ;

    virtual ~CCommand() ;

    void setRdirInFileName(string &s) ;

    int doRdirIn() ;

    void setRdirOutFileName(string &s) ;

    void doRdirOut() ;

    bool execute() ;

    void runNormalCommand(char **argv, int fdIn, int fdOut, int &nRet);

    void runMultiCommand(char **argv_A, char **argv_B, int fdIn, int fdOut, int &nRet) ;

};





#endif //CS100PROJ_CCOMMAND_H
