#ifndef RSHELL_CPARSER_H
#define RSHELL_CPARSER_H

#include <string>
#include <vector>
#include <sstream>
#include <cstring>
#include <algorithm>


using namespace std;

class CParser {
public:
    static int processComment(string &inputString, bool &bValid, vector<pair<int, int> > &rangeVector, bool &bNeed) {
        size_t prev = 0;
        size_t posPound = 0;
        size_t posLast = 0;

        bool bDone = false;
        char c = 0;
        char t = 0;

        bNeed = false;

        // Remove comment
        do {
            posPound = inputString.find_first_of("#", prev);
            if (posPound != string::npos) {
                size_t posLeft = 0;
                size_t posRight = 0;
                if (posPound) {
                    posLeft = inputString.find_first_of("\"", prev);
                    if ((posLeft != string::npos) && (posLeft < posPound)) {

                        if (posLeft >= 1) {
                            if (inputString[posLeft - 1] == '\\') {

                                prev = posLeft + 1;
                                continue;
                            }
                        }

                        posLast = posPound;
                        do {
                            posRight = inputString.find_first_of("\"", posLast + 1);
                            if (posRight == string::npos) {
                                cout << "Unpaired quotations and comment symbol!" << "Last found at " << posLeft
                                     << endl;
                                return __LINE__;
                            }

                            if (inputString[posRight - 1] == '\\') {
                                posLast = posRight + 1;
                            } else {
                                break;
                            }

                        } while (posRight != string::npos);

                        //inputString.substr(posLeft, posRight-posLeft+1);
                        rangeVector.push_back(make_pair(posLeft, posRight));

                        prev = posRight + 1;

                    } else { // Just extract the preceding
                        inputString = inputString.substr(0, posPound);
                        bDone = true;
                        break;
                    }
                } else { // Only one pound at the first one
                    return __LINE__;
                }
            } else { // No comment at all
                bDone = true;
                bNeed = true;
                break;
            }
        } while (!bDone);

        return 0;
    }

    static int processQuotation(string &inputString, bool &bValid, vector<pair<int, int> > &rangeVector) {
        size_t prev = 0;
        size_t pos = 0;

        bool bDone = false;
        char c = 0;
        char t = 0;

        size_t posLeft = 0;
        size_t posRight = 0;
        const size_t len = inputString.length();

 //       size_t posLeftOld = 0;
//        size_t posRightOld = 0;

        size_t posTemp = 0;

        int unpaired = 0;
        int nLast = 0;

        do {
            pos = inputString.find_first_of("\"", prev);
            if (pos != string::npos) {

//                posLeftOld = posLeft;
//                posLeft = pos;
                if (pos >= 1) {
                    t = inputString[pos - 1];
                    if ( t == '\\') {

                        prev = pos + 1;
                        continue;
                    }
                }

                if(unpaired==0) {
                    unpaired = 1;

                    prev = pos + 1;
                    posTemp = pos;

                    continue;
                }

                unpaired = 0;

                posLeft = posTemp;
                posRight = pos;


//                nLast = posLeft;
//
//                posRightOld = posRight;
//
//                posRight = inputString.find_first_of("\"", pos + 1);
//                if (posRight == string::npos) {
//
//                    if(pos+1!=len){
//                        cout << "Unpaired quotations!" << "Only found left at " << posLeft << endl;
//                        return __LINE__;
//                    }
//                    else{
//                       // cout << "last quotation"<<endl;
//                        posRight =  posLeftOld;
//                    }
//                }
//
//                if (posRight >= 1) {
//                    t = inputString[posRight - 1];
//                    if (t == '\\') {
//
//                        prev = posRight + 1;
//                        continue;
//                    }
//                }
//
//                unpaired--;

                rangeVector.push_back(make_pair(posLeft, posRight));

                prev = posRight + 1;

            } else { // No quotation
                bDone = true;
                break;
            }
        } while (!bDone);

        if (unpaired) {
            cout << "Unpaired quotations!" << "Last found left at " << nLast << endl;
            return __LINE__;
        }

        return 0;
    }

    static int removeBackslash(string &str) {
        str.erase(std::remove(str.begin(), str.end(), '\\'), str.end());
        return 0;
    }

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
        char c = 0;
        char t = 0;
        char pr = 0;
        //vector<char> vecSpecialMark;

        vector<pair<int, int> > rangeVector;

        nRet = processComment(inputString, bValid, rangeVector, bNeedQuotationCheck);
        if (nRet) {
            return nRet;
        }

        if (bNeedQuotationCheck) {
            nRet = processQuotation(inputString, bValid, rangeVector);
            if (nRet) {
                return nRet;
            }
        }

        if (!rangeVector.empty()) {
            reverse(rangeVector.begin(), rangeVector.end());
        }

        stringstream ss(inputString);
        while (getline(ss, line)) {
            // Remove space
            while ((pos = line.find_first_of(" \"", prev)) != string::npos) {
                bValid = false;
                c = line[pos];
                if (pos)
                    pr = line[pos - 1];

                if ((pos > prev) || (pos == prev && t != c)) {
                    tokenTemp = line.substr(prev, pos - prev);
                    len = tokenTemp.length();

                    if (c == '"') {
                        if (pr != '\\') {
                            if (rangeVector.empty()) {
                                cout << "ERR: empty!" << endl;
                                return __LINE__;
                            }

                            size_t posLeft = 0;
                            size_t posRight = 0;
                            pair<int, int> rangeOne = rangeVector.back();
                            rangeVector.pop_back();

                            // Omit quotation
                            posLeft = rangeOne.first;
                            posRight = rangeOne.second;

                            if (posLeft != pos) {
                                cout << "ERR: inconsistent!" << endl;
                                return __LINE__;
                            }

                            posLeft = rangeOne.first + 1;
                            posRight = rangeOne.second - 1;

                            tokenTemp = line.substr(posLeft, posRight - posLeft + 1);

                            pos = posRight;
                            prev = posRight + 1;

                            bValid = true;
                        } else {
                            bValid = false;
                            break;
                        }
                    }

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
                t = c;
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

    static void splitConsecutive(string &tokenChunk, vector<string> &wordVector) {

        bool bToSplit = false;
        char c = 0;
        string strGenuine;

        const size_t len = tokenChunk.length();
        size_t prev = 0;
        size_t pos = 0;

        pos = tokenChunk.find_first_of(";<>&|", 0);
        if (pos == string::npos) {
            wordVector.push_back(tokenChunk);
            return;
        }

        while ((pos = tokenChunk.find_first_of(";<>&|", prev)) != string::npos) {
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

    static char **vectorToArgvWithExtra(vector<string> &vecToken, int nExtra, vector<string> &vecExtra) {
        char **argv = nullptr;
        int size = vecToken.size();

        if (size) {
            argv = new char *[size + 1 + nExtra];
            argv[size + nExtra] = nullptr;

            for (int i = 0; i < size; ++i) {

                string itStr = vecToken[i];
                argv[i] = new char[itStr.length() + 1];
                strcpy(argv[i], itStr.c_str());
            }

            for (int j = size; j < size + nExtra; j++) {
                string itStr = vecExtra[j];
                argv[j] = new char[itStr.length() + 1];
                strcpy(argv[j], itStr.c_str());
            }
        }

        return argv;
    }


    static void cleanUpArgv(char **argv, int size) {

        // clean up
        for (int i = 0; i < size; ++i) {
            delete[] argv[i];
        }

        delete[] argv;
    }
};


#endif //RSHELL_CPARSER_H
