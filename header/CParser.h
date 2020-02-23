#ifndef RSHELL_CPARSER_H
#define RSHELL_CPARSER_H

#include <string>
#include <vector>
#include <sstream>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <unordered_map>

using namespace std;

class CParser {
public:

    // Analyze the complexity
    // 1 - Comment only  2 - Quotations only
    static int analyzeComplexity(const string &inputString) {
        int nResult = 0;

        size_t posPound = inputString.find_first_of("#", 0);
        if (posPound != string::npos) {
            nResult |= 1;
        }

        size_t posPrev = 0;
        size_t posQuotation = 0;

//        do{
//            posQuotation = inputString.find_first_of("\"", posPrev);
//            if(posQuotation==string::npos)
//                break;
//
//            if(posQuotation>=1){
//                if(inputString[posQuotation-1]=='\\') {
//                    nResult |= 4;
//                    break;
//                } else{
//                    nResult |= 2;
//                }
//            }
//
//            posPrev = posQuotation+1;
//
//        }while(posQuotation!=string::npos);

        posQuotation = inputString.find_first_of("\"", posPrev);
        if (posQuotation == string::npos)
            nResult |= 2;

        return nResult;
    }

    /*
     * Remove comment only. Remove the part after the pound symbol.
     */
    static int removeCommentOnly(string &inputString) {

        size_t posPound = inputString.find_first_of("#", 0);
        if (posPound != string::npos) {
            inputString = inputString.substr(0, posPound);
        }

        return 0;
    }


    /*
     * Remove comments before output
     */
    static int processComment(string &inputString, bool &bNeedOtherCheck) {
        size_t prevPound = 0;
        size_t posPound = 0;

        size_t posLeft = 0;
        size_t posRight = 0;

        size_t prevLeft = 0;
        size_t prevRight = 0;

        bool bDone = false;
        char c = 0;
        char t = 0;

        int nCount = 0;

        do {
            posPound = inputString.find_first_of("#", prevPound);

            if (posPound == string::npos) {
                bDone = true;
                bNeedOtherCheck = true;
                break;
            }

            if (!posPound) {
                bDone = true;
                bNeedOtherCheck = false;
                inputString = "";
                break;
            }

            nCount = 0;
            prevLeft = 0;

            do {
                posLeft = inputString.find_first_of("\"", prevLeft);

                if (posLeft == string::npos || posLeft >= posPound) {
                    break;
                }

                if (posLeft >= 1) {
                    if (inputString[posLeft - 1] == '\\') {

                    } else {
                        nCount++;
                    }
                }

                prevLeft = posLeft + 1;

            } while (posLeft != string::npos);

            // All quotations on left sides
            if (nCount % 2 == 0) {
                inputString = inputString.substr(0, posPound);
                bDone = true;
                bNeedOtherCheck = true;
                break;
            }

            prevRight = posPound;

            do {
                posRight = inputString.find_first_of("\"", prevRight);
                if (posRight == string::npos) {

                    break;
                }

                if (inputString[posRight - 1] == '\\') {

                } else {
                    nCount++;

                    break;
                }

                prevRight = posRight + 1;

            } while (posRight != string::npos);

            if (nCount % 2) {
                cout << "Unpaired quotations and comment symbol!" << "Last found at " << posLeft << endl;
                return __LINE__;
            }
            prevPound = posRight + 1;

        } while (!bDone);

        return 0;
    }

    /*
     * Mark inner part of quotations
     */
    static int
    processQuotation(string &inputString, vector<pair<int, int> > &rangeVector, vector<pair<int, int> > &rangeVectorB) {
        size_t prev = 0;
        size_t pos = 0;
        size_t posLast = 0;

        size_t prevB = 0;

        bool bDone = false;
        char c = 0;

        int nCount = 0;
        int nCountB = 0;

        do {
            pos = inputString.find_first_of("\"", prev);
            if (pos == string::npos) {
                bDone = true;
                break;
            }

            if (pos >= 1) {
                if (inputString[pos - 1] == '\\') {

                    nCountB++;

                    if (nCountB && nCountB % 2 == 0) {
                        rangeVectorB.push_back(make_pair(prevB, pos));
                    }

                    prevB = pos;
                    prev = pos + 1;


                    continue;
                }
            }

            nCount++;

            if (nCount && nCount % 2 == 0) {
                rangeVector.push_back(make_pair(posLast, pos));
            }

            posLast = pos;
            prev = pos + 1;

        } while (!bDone);

        if (nCount % 2) {
            cout << "Unpaired quotations!" << "Last found left at " << posLast << endl;
            return __LINE__;
        }

        if (nCountB % 2) {
            cout << "Unpaired escape quotations !" << "Last found left at " << prevB << endl;
            return __LINE__;
        }

        return 0;
    }


    /*
    * Replace brackets
    */
    static int processBracket(string &inputString) {
        vector<pair<int, int> > rangeVector;
        vector<int> posVector;

        string strDup;

        char chCurSymbol = 0;
        char chTemp = 0;

        size_t prev = 0;
        size_t pos = 0;
        size_t tempPos = 0;

        bool bDone = false;
        char c = 0;
        char t = 0;

        size_t posLeft = 0;
        size_t posRight = 0;
        const size_t len = inputString.length();

        size_t posTemp = 0;

        int unpaired = 0;
        int nLast = 0;

        do {
            pos = inputString.find_first_of("[]", prev);
            if (pos != string::npos) {
                chCurSymbol = inputString[pos];

                if (pos >= 1) {
                    t = inputString[pos - 1];
                    if (t == '\\') {

                        prev = pos + 1;
                        continue;
                    }
                }

                if (chCurSymbol == '[') {
                    posVector.push_back(pos);
                } else if (chCurSymbol == ']') {

                    tempPos = posVector.back();
                    posVector.pop_back();

                    rangeVector.push_back(make_pair(tempPos, pos));
                }

                prev = pos + 1;

            } else {
                bDone = true;
                break;
            }
        } while (!bDone);

        if (posVector.size()) {
            cout << "Unpaired brackets!" << "Last found left at " << nLast << endl;
            return __LINE__;
        }

        prev = 0;
        if (!rangeVector.empty()) {

            for (int i = 0; i < rangeVector.size(); i++) {

                pair<int, int> pr = rangeVector[i];
                posLeft = pr.first;
                posRight = pr.second;

                strDup += inputString.substr(prev, posLeft - prev);
                strDup += "test ";
                strDup += inputString.substr(posLeft + 1, posRight - posLeft - 1);

                prev = posRight + 1;
            }

            strDup += inputString.substr(prev);

            inputString = strDup;
        }
        return 0;
    }


    static int removeBackslash(string &str) {
        str.erase(std::remove(str.begin(), str.end(), '\\'), str.end());
        return 0;
    }


    static int extractInnerPart(const char pr, const string & line, string & tokenTemp, size_t & pos,  vector<pair<int, int> > &rangeVector,  vector<pair<int, int> > &rangeVectorB){

        int nSearchMode = 0;

        //string line = lineOrg;

        vector<pair<int, int> >* pVecA;
        vector<pair<int, int> >* pVecB;

        size_t posLeft = 0;
        size_t posRight = 0;

        size_t posL = 0;
        size_t posR = 0;

        if (pr != '\\') {
            nSearchMode = 0;
            pVecA = &rangeVector;
            pVecB = &rangeVectorB;
        }
        else{
            nSearchMode = 1;
            pVecA = &rangeVectorB;
            pVecB = &rangeVector;
        }

        if (pVecA->empty()) {
            cout << "ERR: empty!" << endl;
            return __LINE__;
        }


        pair<int, int> rangeOne = pVecA->back();
        pVecA->pop_back();

        posLeft = rangeOne.first;
        posRight = rangeOne.second;

        if (posLeft != pos) {
            cout << "ERR: inconsistent!" << endl;
            return __LINE__;
        }

        pos = posRight;

        // Omit preceding
        if(nSearchMode==0){
            posLeft += 1;
            posRight -= 1;

            tokenTemp = line.substr(posLeft, posRight - posLeft + 1);
        }
        else{
            posRight -= 2;
            tokenTemp = line.substr(posLeft, posRight - posLeft + 1);
            tokenTemp += "\"";
        }


        //--------------------------------------------------
        unordered_map <int, bool> mapRM;


        for(int i=0;i<pVecB->size();i++){

            pair<int, int> rangeIn = pVecB->at(i);

            posL = rangeIn.first;
            posR = rangeIn.second;

            if(posL > posLeft && posR < posRight ){

                if(nSearchMode==0){

                    int nSubPos = 0;
                    int nSubLen = (posL-1) - posLeft ;

                    string strA = tokenTemp.substr(nSubPos, nSubLen);

                    nSubPos = posL - posLeft;
                    nSubLen = (posR-1) - posL ;

                    string strB = tokenTemp.substr(nSubPos, nSubLen);


                    nSubPos = posR- posLeft;
                    nSubLen = posRight-posR+1;

                    string strC = tokenTemp.substr(nSubPos, nSubLen);

                    tokenTemp = strA + strB + strC;
                }
                else{

                    int nSubPos = 0;
                    int nSubLen = posL-posLeft ;

                    string strA = tokenTemp.substr(nSubPos, nSubLen);

                    nSubPos = posL - posLeft +1;
                    nSubLen = (posR-1) - posL;

                    string strB = tokenTemp.substr(nSubPos, nSubLen);


                    nSubPos = posR+1-posLeft;
                    nSubLen = posRight-posR;

                    string strC = tokenTemp.substr(nSubPos, nSubLen);

                    tokenTemp = strA + strB + strC+"\"";
                }


                //line[posL]=' ';
                //line[posR]=' ';

                mapRM[i] = true;
            }
        }

        vector<pair<int, int>> vecPairTemp;

        if(mapRM.size()){
            int i;
            for(i=0;i<pVecB->size();i++){

                if(!mapRM[i])
                    vecPairTemp.push_back(pVecB->at(i));
            }

            pVecB->clear();
            *pVecB = vecPairTemp;
        }

        return 0;

    }


    /*
     * Detect the tokens
     */
    static int parserLineToVector(const char *pFullLine, vector<string> &wordVector) {
        int nRet = 0;
        string inputString = pFullLine;

        string line;

        string tokenTemp;

        size_t prev = 0;
        size_t pos = 0;
        size_t len = 0;

        bool bValid = false;
        bool bDone = false;
        bool bNeedQuotationCheck = false;
        char chCurrent = 0;
        char chPrev = 0;
        char pr = 0;

        vector<pair<int, int> > rangeVector;
        vector<pair<int, int> > rangeVectorB;

        nRet = processBracket(inputString);
        if (nRet) {
            return nRet;
        }

        nRet = processComment(inputString, bNeedQuotationCheck);
        if (nRet) {
            return nRet;
        }

        if (bNeedQuotationCheck) {
            nRet = processQuotation(inputString, rangeVector, rangeVectorB);
            if (nRet) {
                return nRet;
            }
        }

        if (!rangeVector.empty()) {
            reverse(rangeVector.begin(), rangeVector.end());
        }

        if (!rangeVectorB.empty()) {
            reverse(rangeVectorB.begin(), rangeVectorB.end());
        }

        stringstream ss(inputString);
        while (getline(ss, line)) {

            while ((pos = line.find_first_of(" \"", prev)) != string::npos) {
                bValid = false;
                chCurrent = line[pos];

                if (pos)
                    pr = line[pos - 1];

                if ((pos > prev) || (pos == prev && pos > 0 && chPrev != chCurrent)) {

                    tokenTemp = line.substr(prev, pos - prev);
                    len = tokenTemp.length();

                    if (chCurrent == '"') {

                        nRet = extractInnerPart(pr, line, tokenTemp, pos, rangeVector, rangeVectorB);
                        if(nRet)
                            return nRet;

                        bValid = true;
                    }
                }

                if(tokenTemp.length()){
                    if (len == 1 || bValid) {
                        wordVector.push_back(tokenTemp);
                    } else if (len == 2) {
                        if (tokenTemp == "&&" || tokenTemp == "||" || tokenTemp == "<<" || tokenTemp == ">>") {
                            wordVector.push_back(tokenTemp);
                        } else {
                            splitConsecutive(tokenTemp, wordVector);
                        }
                    } else {
                        splitConsecutive(tokenTemp, wordVector);
                    }
                }


                chPrev = chCurrent;
                prev = pos + 1;
            }

            if (prev < line.length()) {
                tokenTemp = line.substr(prev, string::npos);
                //wordVector.push_back(token);
                splitConsecutive(tokenTemp, wordVector);
            }
        }

        return 0;
    }

/*
 * Split the consecutive commands
 */
    static
    void splitConsecutive(string &tokenChunk, vector<string> &wordVector) {

        bool bToSplit = false;
        char c = 0;
        string strGenuine;

        const size_t len = tokenChunk.length();
        size_t prev = 0;
        size_t pos = 0;

        pos = tokenChunk.find_first_of(";<>&|()", 0);
        if (pos == string::npos) {
            wordVector.push_back(tokenChunk);
            return;
        }

        while ((pos = tokenChunk.find_first_of(";<>&|()", prev)) != string::npos) {
            c = tokenChunk[pos];

            strGenuine = tokenChunk.substr(prev, pos - prev);
            if (strGenuine.length()) {
                wordVector.push_back(strGenuine);
            }

            // Current part
            if (pos < len) {
                char sz[3] = {0, 0, 0};
                sz[0] = c;

                if (pos < len - 1) {
                    if (c == tokenChunk[pos + 1]) {
                        if (c == '&' || c == '|') {
                            sz[1] = c;
                            pos++;
                        } else if (c == ';' || c == '<' || c == '>') {
                            pos++;
                        }
                    }
                }
                wordVector.push_back(sz);
            }

            prev = pos + 1;
        }

        if (prev < len) {
            string strTail = tokenChunk.substr(prev, string::npos);
            wordVector.push_back(strTail);
        }
    }


/*
 * Create the argument array
 */
    static char **vectorToArgv(vector<string> &vecToken) {
        char **argv = nullptr;
        int size = vecToken.size();

        if (size) {
            argv = new char *[size + 1];
            argv[size] = nullptr;

            for (int i = 0; i < size; ++i) {

                string itStr = vecToken[i];
                argv[i] = new char[itStr.length() + 1];
                strcpy(argv[i], itStr.c_str());
            }
        }

        return argv;
    }


/*
 * Clean up the argument array
 */
    static void cleanUpArgv(char **argv, int size) {

        // clean up
        for (int i = 0; i < size; ++i) {
            delete[] argv[i];
            argv[i] = nullptr;
        }

        delete[] argv;
        argv = nullptr;
    }

};


#endif //RSHELL_CPARSER_H
