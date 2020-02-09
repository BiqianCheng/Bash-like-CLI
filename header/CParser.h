#ifndef RSHELL_CPARSER_H
#define RSHELL_CPARSER_H

#include <string>
#include <vector>
#include <sstream>
#include <cstring>

using namespace std;

class CParser {
public:
    static int ParserLineToVector(const char * pFullLine, vector<string> & wordVector){

        string inputString = pFullLine;

        string line;

        string tokenTemp;

        size_t prev = 0;
        size_t pos = 0;
        size_t len = 0;

        // Remove comment
        pos = inputString.find_first_of("#", 0);
        if (pos != string::npos) {
            if (pos) {
                inputString = inputString.substr(0, pos);
            } else {
                return __LINE__;
            }
        }

        stringstream ss(inputString);
        while (getline(ss, line)) {
            // Remove space and quotation
            while ((pos = line.find_first_of(" '\"", prev)) != string::npos) {
                if (pos > prev) {
                    tokenTemp = line.substr(prev, pos - prev);

                    len = tokenTemp.length();

                    if(len==1){
                         wordVector.push_back(tokenTemp);
                    }
                    else if(len==2){
                        if( tokenTemp== "&&" || tokenTemp== "||" || tokenTemp== "<<" || tokenTemp== ">>" ){
                            wordVector.push_back(tokenTemp);
                        }
                        else {
                            splitConsecutive(tokenTemp, wordVector);
                        }
                    }
                    else{
                        splitConsecutive(tokenTemp, wordVector);
                    }
                }
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

    static void splitConsecutive(string & tokenChunk, vector<string> & wordVector){

        bool bToSplit = false;
        char c = 0;
        string strGenuine;

        const size_t len = tokenChunk.length();
        size_t prev = 0;
        size_t pos = 0;

        pos = tokenChunk.find_first_of(";<>&|", 0);
        if(pos == string::npos){
            wordVector.push_back(tokenChunk);
            return;
        }

        while ((pos = tokenChunk.find_first_of(";<>&|", prev)) != string::npos) {
            c = tokenChunk[pos];

            strGenuine = tokenChunk.substr(prev, pos - prev);
            if(strGenuine.length()){
                wordVector.push_back(strGenuine);
            }

            // Current part
            if(pos<len){
                char sz[3]={0,0,0};
                sz[0] = c;

                if(pos<len-1) {
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


    static char ** vectorToArgv(vector<string> & vecToken ){
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

    static char ** vectorToArgvWithExtra(vector<string> & vecToken, int nExtra, vector<string> & vecExtra ){
        char **argv = nullptr;
        int size = vecToken.size();

        if (size) {
            argv = new char *[size + 1+nExtra];
            argv[size+nExtra] = nullptr;

            for (int i = 0; i < size; ++i) {

                string itStr = vecToken[i];
                argv[i] = new char[itStr.length() + 1];
                strcpy(argv[i], itStr.c_str());
            }

            for(int j=size;j<size+nExtra;j++){
                string itStr = vecExtra[j];
                argv[j] = new char[itStr.length() + 1];
                strcpy(argv[j], itStr.c_str());
            }
        }

        return argv;
    }


    static void cleanUpArgv(char **argv, int size){

        // clean up
        for (int i = 0; i < size; ++i) {
            delete[] argv[i];
        }

        delete[] argv;
    }
};


#endif //RSHELL_CPARSER_H
