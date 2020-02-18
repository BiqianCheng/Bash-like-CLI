#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include <unistd.h>

#include "../header/CAndConnector.h"
#include "../header/CCommand.h"
#include "../header/COrConnector.h"
#include "../header/CSeparatorConnector.h"
#include "../header/CRunMode.h"
#include "../header/CRdirConnector.h"
#include "../header/CPipeConnector.h"


//#define _MY_DEBUG

#define USE_INTERNAL_DEBUG

using namespace std;

CRunMode::RunningMode  CRunMode::nMode = CRunMode::R_NORMAL;

CConnector *parseLineToExecutor(const string &inputConstString);

/*
 * Parser command
 *
 */
int parser() {
    int nRet = 0;
    CConnector *pUltimateConnector = nullptr;
    //vector<string> wordVector;

    string strInput;
    cout << "$";

#ifdef USE_INTERNAL_DEBUG
    static int nCnt = 0;
#else
    static int nCnt = 1;
#endif

    if (!nCnt) {
        //strInput = "echo \"       # other\"";
        //strInput = "echo         # other";
        //strInput = "echo    z     # other";
        //strInput = "echo    \"# one\"  \"# two\"  ";
        //strInput = "echo    \\\" orphan  ";
        //strInput = "echo    \" \\\"# one\\\"  \"# two\"  ";
        //strInput = "echo \"------------Commented Command Tests------------\"";
        //strInput = "echo aa;echo bb;";
        //strInput = "ls > 1.txt";
        //strInput = "tr a-z A-Z < 1.txt";
        //strInput = "ls -l | wc -l";
        //strInput = "echo one && echo two; ";
        //strInput = "echo one";
        //strInput = "test -e test/file/path && echo “path exists”";
        //strInput = "[ -e test/file/path ] && echo “path exists”";
        //strInput = "[ -f rshell ] && echo \"file exists\"";
        strInput = "[ -f rshell ] && test -e test/file/path ";
    } else {
        getline(cin, strInput);
    }

    nCnt++;

    //parseLineToExecutor(strInput, wordVector);
    pUltimateConnector = parseLineToExecutor(strInput);

    if (pUltimateConnector) {
        pUltimateConnector->execute();

        CConnector::cleanIt(pUltimateConnector);
        pUltimateConnector = nullptr;
    }


    return nRet;
}


//class CException {
//
//    string strDesc;
//public:
//    CException(const char *p) {
//        strDesc = p;
//    }
//};

/*
 * Create connectors with original connectors and incoming commands
 */
void ConnectorStackOp(vector<CConnector *> &connectorVector, CConnector *pTempConnector, vector<string> &cmdArgVector) {

    CCommand *pCmd;
    pCmd = new CCommand();

    for (int i = 0; i < cmdArgVector.size(); i++) {
        pCmd->vecToken.push_back(cmdArgVector[i]);
    }

    if (connectorVector.empty()) {
        pTempConnector->leftSideItems = pCmd;
        connectorVector.push_back(pTempConnector);
    } else {
        CConnector *pTop = connectorVector.back();
        connectorVector.pop_back();

        pTop->rightSideItems = pCmd;

        pTempConnector->leftSideItems = pTop;
        connectorVector.push_back(pTempConnector);
    }
}

/*
 * Parse the string into executor
 */
CConnector *parseLineToExecutor(const string &inputConstString) {
    int pos = 0;
    string token;
    vector<string> wordVector;
    vector<string> cmdArgVector;
    vector<CConnector *> connectorVector;

    CConnector *pUltimateConnector = nullptr;

    CParser::parserLineToVector(inputConstString.c_str(), wordVector);

#ifdef  _MY_DEBUG
    for_each(wordVector.begin(), wordVector.end(), [](const string &its) { cout << its << endl; });
#endif

    reverse(wordVector.begin(), wordVector.end());

    while (!wordVector.empty()) {
        token = wordVector.back();
        wordVector.pop_back();

        if (token == "&&") {

            CAndConnector *pTempConnector = new CAndConnector();
            ConnectorStackOp(connectorVector, pTempConnector, cmdArgVector);
            cmdArgVector.clear();

        } else if (token == "||") {

            COrConnector *pTempConnector = new COrConnector();
            ConnectorStackOp(connectorVector, pTempConnector, cmdArgVector);
            cmdArgVector.clear();

        } else if (token == ";") {

            CSeparatorConnector *pTempConnector = new CSeparatorConnector();
            ConnectorStackOp(connectorVector, pTempConnector, cmdArgVector);
            cmdArgVector.clear();

        } else if (token == "<" || token == "<<") {

            CRdirInConnector *pTempConnector = new CRdirInConnector();
            ConnectorStackOp(connectorVector, pTempConnector, cmdArgVector);
            cmdArgVector.clear();

        } else if (token == ">" || token == ">>") {
            CRdirOutConnector *pTempConnector = new CRdirOutConnector();
            ConnectorStackOp(connectorVector, pTempConnector, cmdArgVector);
            cmdArgVector.clear();

        } else if (token == "|") {

            CPipeConnector *pTempConnector = new CPipeConnector();
            ConnectorStackOp(connectorVector, pTempConnector, cmdArgVector);
            cmdArgVector.clear();
        } else {
            cmdArgVector.push_back(token);
        }
    }



    if (!cmdArgVector.empty()) {
        CConnector *pTempConnector = new CConnector;
        ConnectorStackOp(connectorVector, pTempConnector, cmdArgVector);
        pUltimateConnector = pTempConnector;
    }
    else{
        if(!connectorVector.empty()) {
            CConnector *pTempConnector = connectorVector.back();
            pUltimateConnector = pTempConnector;
        }
        else{ // empty command
            CConnector *pTempConnector = new CConnector();
            pUltimateConnector = pTempConnector;
        }
    }

    //connectorVector.clear();

    return pUltimateConnector;
}





/*
 * The main part of the rshell
 */
int main() {

    do {
        parser();
        sleep(1);

        if (CRunMode::isEnding())
            break;

    } while (1);

    return 0;
}