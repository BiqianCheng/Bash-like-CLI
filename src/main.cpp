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

//#define USE_INTERNAL_DEBUG

using namespace std;

CRunMode::RunningMode  CRunMode::nMode = CRunMode::R_NORMAL;

CConnector *parseLineToExecutor(const string &inputConstString,  CConnector * pRHS);

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
        //strInput = "test -e test/file/path && echo \"path exists\"";
        //strInput = "[ -e test/file/path ] && echo \"path exists\"";
        //strInput = "[ -f rshell ] && echo \"file exists\"";
        //strInput = "[ -f rshell ] && test -e test/file/path ";
        //strInput = "(echo aa && echo bb) || (echo cc && echo dd)";
        //strInput = "(echo aa && echo bb) && (echo cc || echo dd)";
        //strInput = "(echo aa && echo bb) && (echo cc && echo dd)";
        //strInput = "(echo aa || echo bb) && (echo cc || echo dd)";
        //strInput = "(echo aa || echo bb) && (echo cc && (echo dd && echo ee))";
        //strInput = "(echo aa || echo bb) && (echo cc && (echo dd && vvv))";
        //strInput = "(echo aa || echo bb) && (echo cc && (vvv || echo ee))";

        //strInput = "(echo cc && (echo dd && echo ee) && (echo aa || echo bb) )";


        //             echo \"12345"------"6789\" "two" "three"
        //strInput = " echo \\\"12345\"------\"6789\\\" \"two\" \"three\"  ";

        //strInput = " echo \\\"12345\\\" ";
        //strInput = " echo \"12345\" ";

        //             echo "1234\"--------\"5678"
        //strInput = " echo \"1234\\\"--------\\\"5678\" ";

        // echo "a\"b\"c"
        //strInput = "echo \"a\\\"b\\\"c\"";
        //strInput = "echo \\\"a\"b\"c\\\"";
    } else {
        getline(cin, strInput);
    }

    nCnt++;

    //parseLineToExecutor(strInput, wordVector);
    pUltimateConnector = parseLineToExecutor(strInput, nullptr);

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
void ConnectorStackOp(vector<CConnector *> &connectorVector, CConnector *pTempConnector, vector<string> &cmdArgVector,
                      int &nLevel, bool bClosed) {

    CCommand *pCmd = nullptr;

    if(cmdArgVector.size()) {
        pCmd = new CCommand();

        for (int i = 0; i < cmdArgVector.size(); i++) {
            pCmd->vecToken.push_back(cmdArgVector[i]);
        }
    }

    if (connectorVector.empty()) {
        if(!pTempConnector)
            pTempConnector = new CConnector;

        pTempConnector->leftSideItems = pCmd;
        connectorVector.push_back(pTempConnector);

    } else {
        CConnector *pTop = connectorVector.back();
        connectorVector.pop_back();

        if(pCmd)
            pTop->rightSideItems = pCmd;

        if(!pTempConnector)
            pTempConnector = new CConnector;

        pTempConnector->leftSideItems = pTop;

        connectorVector.push_back(pTempConnector);
    }
}

/*
 * Parse the string into executor
 */
CConnector *parseLineToExecutor(const string &inputConstString, CConnector * pRHS) {
    int pos = 0;
    int nLevel = 0;
    string token;
    string strChild[256];

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

        if(nLevel) {

            if (token == "(") {

                nLevel++;

            } else if (token == ")") {

                CConnector * pChild = parseLineToExecutor(strChild[nLevel], NULL);
                strChild[nLevel].clear();
                nLevel--;

                while(nLevel) {
                    pChild = parseLineToExecutor(strChild[nLevel], pChild );
                    nLevel--;
                }

                if (connectorVector.size()) {
                    CConnector *pTop = connectorVector.back();

                    pTop->rightSideItems = pChild;
                } else {
                    connectorVector.push_back(pChild);
                }

            }
            else{
                strChild[nLevel] += token;
                strChild[nLevel] += " ";
            }
        }
        else{

            if (token == "&&") {

                CAndConnector *pTempConnector = new CAndConnector();
                ConnectorStackOp(connectorVector, pTempConnector, cmdArgVector, nLevel,false);
                cmdArgVector.clear();

            } else if (token == "||") {

                COrConnector *pTempConnector = new COrConnector();
                ConnectorStackOp(connectorVector, pTempConnector, cmdArgVector, nLevel,false);
                cmdArgVector.clear();

            } else if (token == ";") {

                CSeparatorConnector *pTempConnector = new CSeparatorConnector();
                ConnectorStackOp(connectorVector, pTempConnector, cmdArgVector, nLevel,false);
                cmdArgVector.clear();

            } else if (token == "<" || token == "<<") {

                CRdirInConnector *pTempConnector = new CRdirInConnector();
                ConnectorStackOp(connectorVector, pTempConnector, cmdArgVector, nLevel,false);
                cmdArgVector.clear();

            } else if (token == ">" || token == ">>") {
                CRdirOutConnector *pTempConnector = new CRdirOutConnector();
                ConnectorStackOp(connectorVector, pTempConnector, cmdArgVector, nLevel,false);
                cmdArgVector.clear();

            } else if (token == "|") {

                CPipeConnector *pTempConnector = new CPipeConnector();
                ConnectorStackOp(connectorVector, pTempConnector, cmdArgVector, nLevel,false);
                cmdArgVector.clear();

            } else if (token == "(") {

                nLevel++;

            } else if (token == ")") {

                //nLevel--;
            } else {
                cmdArgVector.push_back(token);
            }
        }
    }

    if (!cmdArgVector.empty()) {
        //CConnector *pTempConnector = new CConnector;
        //ConnectorStackOp(connectorVector, pTempConnector, cmdArgVector, nLevel, false);
        //pUltimateConnector = pTempConnector;
        ConnectorStackOp(connectorVector, nullptr, cmdArgVector, nLevel, false);
    }


    if (!connectorVector.empty()) {
        CConnector *pTempConnector = connectorVector.back();
        pUltimateConnector = pTempConnector;
    } else { // empty command
        CConnector *pTempConnector = new CConnector();
        pUltimateConnector = pTempConnector;
        //cout << "Empty connector!"<<endl;
    }


    if(pRHS){
        if(!pUltimateConnector->rightSideItems){
            pUltimateConnector->rightSideItems = pRHS;
        }
        else{
            cout << "Nested error."<<endl;
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
