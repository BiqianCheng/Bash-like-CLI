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


void ConnectorStackOp(vector<CConnector *> &cmdStack, CConnector *pTempConnector, vector<string> &cmdArgVector) {

    CCommand *pCmd;
    pCmd = new CCommand();
    pCmd->vecToken = cmdArgVector;
    cmdArgVector.clear();

    if (cmdStack.empty()) {
        pTempConnector->leftSideItems = pCmd;
        cmdStack.push_back(pTempConnector);
    } else {
        CConnector *pTop = cmdStack.back();
        cmdStack.pop_back();

        pTop->rightSideItems = pCmd;

        pTempConnector->leftSideItems = pTop;
        cmdStack.push_back(pTempConnector);
    }
}


CConnector * parseCommandLineAndExecute(const string &inputString) {

    stringstream stringStream(inputString);
    string line;
    vector<string> wordVector;
    vector<string> cmdArgVector;
    string token;

    vector<CConnector * > cmdStack;

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

        if(token == "&&" ){

            CAndConnector *pTempConnector = new CAndConnector();
            ConnectorStackOp(cmdStack, pTempConnector, cmdArgVector);
        }
        else if(token == "||"){

            COrConnector *pTempConnector = new COrConnector();
            ConnectorStackOp(cmdStack, pTempConnector, cmdArgVector);
        }
        else if(token == ";"){

            CSeparatorConnector *pTempConnector = new CSeparatorConnector();
            ConnectorStackOp(cmdStack, pTempConnector, cmdArgVector);
        }
        else{
            cmdArgVector.push_back(token);
        }
    }

    if(!cmdArgVector.empty()) {
        CConnector * pTempConnector = new CConnector;
        ConnectorStackOp(cmdStack, pTempConnector, cmdArgVector);
        pUltimateConnector = pTempConnector;
    }

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

    if (pUltimateConnector)
        pUltimateConnector->execute();

    return nRet;
}


int CRunMode::nMode = 0;

int main() {

    int nRet = 0;
    do {
        parser();
        sleep(1);
    } while (!nRet);

    return 0;
}