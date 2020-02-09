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

using namespace std;

//class CException {
//
//    string strDesc;
//public:
//    CException(const char *p) {
//        strDesc = p;
//    }
//};


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


CConnector *parseLineToExecutor(const string &inputConstString) {
    int pos = 0;
    string token;
    vector<string> wordVector;
    vector<string> cmdArgVector;
    vector<CConnector *> connectorVector;

    CConnector *pUltimateConnector = nullptr;

    CParser::ParserLineToVector(inputConstString.c_str(), wordVector);

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
        CConnector *pTempConnector = connectorVector.back();
        pUltimateConnector = pTempConnector;
    }

    //connectorVector.clear();

    return pUltimateConnector;
}


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

    static int nOnce = 10;
    if (!nOnce) {
        strInput = "echo aa;echo bb;";
        //strInput = "ls > 1.txt";
        //strInput = "tr a-z A-Z < 1.txt";
        //strInput = "ls -l | wc -l";
        nOnce++;
    } else {
        getline(cin, strInput);
    }

    //strInput = "echo one && echo two; ";
    //strInput = "echo one";

    //parseLineToExecutor(strInput, wordVector);
    pUltimateConnector = parseLineToExecutor(strInput);

    if (pUltimateConnector) {
        pUltimateConnector->execute();

        CConnector::cleanIt(pUltimateConnector);
        pUltimateConnector = nullptr;
    }


    return nRet;
}


CRunMode::RunningMode  CRunMode::nMode = CRunMode::R_NORMAL;

int main() {

    do {
        parser();
        sleep(1);

        if (CRunMode::isEnding())
            break;

    } while (1);

    return 0;
}