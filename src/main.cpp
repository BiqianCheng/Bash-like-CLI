#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>


#include <cstdlib>
#include <unistd.h>
#include <stdio.h>
#include <cstring>
#include "../header/CAndConnector.h"
#include "../header/CCommand.h"
#include "../header/COrConnector.h"
#include "../header/CSeparatorConnector.h"
#include "../header/CRunMode.h"


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


CConnector * parseCommandLineAndExecute(const string &inputString) {

    stringstream stringStream(inputString);
    string line;
    vector<string> wordVector;
    vector<string> cmdArgVector;
    string token;

    vector<CConnector *> connectorVector;

    CConnector *pUltimateConnector = nullptr;

    while (getline(stringStream, line)) {
        size_t prev = 0, pos;

        while ((pos = line.find_first_of(" '", prev)) != string::npos) {
            if (pos > prev) {
                token = line.substr(prev, pos - prev);
                wordVector.push_back(token);
            }
            prev = pos + 1;
        }

        if (prev < line.length()) {
            token = line.substr(prev, string::npos);
            wordVector.push_back(token);
        }
    }

#ifdef  _MY_DEBUG
    for_each(wordVector.begin(), wordVector.end(), [](const string &its) { cout << its << endl; });
#endif

    reverse(wordVector.begin(),wordVector.end());

    while(!wordVector.empty()){
        token = wordVector.back();
        wordVector.pop_back();

        if (token == "#") {
            wordVector.clear();
            break;
        } else if (token == "&&") {

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

        } else {
            cmdArgVector.push_back(token);
        }
    }

    if (!cmdArgVector.empty()) {
        CConnector *pTempConnector = new CConnector;
        ConnectorStackOp(connectorVector, pTempConnector, cmdArgVector);
        pUltimateConnector = pTempConnector;
    }

    connectorVector.clear();

    return pUltimateConnector;
}


/*
 * Parser command
 * return 1 for quit
 */
int parser() {
    int nRet = 0;
    CConnector * pUltimateConnector = nullptr;
    //vector<string> wordVector;

    string strInput;
    cout << "$";
    getline(cin, strInput);

    //strInput = "echo one && echo two; ";
    //strInput = "echo one";

    //parseCommandLineAndExecute(strInput, wordVector);
    pUltimateConnector = parseCommandLineAndExecute(strInput);

    if (pUltimateConnector) {
        pUltimateConnector->execute();

        CConnector::cleanIt(pUltimateConnector);
        pUltimateConnector = nullptr;
    }


    return nRet;
}


int CRunMode::nMode = 0;

int main() {

    do {
        parser();
        sleep(1);

        if (CRunMode::isEnding())
            break;

    } while (1);

    return 0;
}