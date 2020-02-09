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
        string token;

        size_t prev = 0;
        size_t pos = 0;

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

        return 0;
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
