#ifndef RSHELL_CTESTCMD_H
#define RSHELL_CTESTCMD_H

#include <sys/stat.h>
#include <iostream>

#include "CCommand.h"


class CTestCmd : public CCommand {
private:    
    int nMode;  // -e 0, -f 1, -d 2
    const char* path;
    const string strModes[3] = {"-e","-f","-d"};

    void setMode() { // Sets flag to 0,1,2 if it finds a flag
        bool found = false;
        int size = vecToken.size();
        for(int i = 0; i < size && !found; i++) {
            for(int j = 0; j < 3; j++) {
                if(strModes[j] == vecToken[i]) {
                    nMode = j;
                    found = true;

					path = vecToken[size-i].c_str();

                    break;
                }
            }
        }

        if(!found) { //If no flag found, assume file is at pos 1
			nMode = 0;
		 	path = vecToken[1].c_str();
		}
    }

public:
    CTestCmd():CCommand() {}
    CTestCmd(const char *pFullLine):CCommand(pFullLine){ }
    CTestCmd(vector<string> & v ):CCommand(v) {}

    virtual ~CTestCmd() {}


    bool execute(){
        setMode();

        struct stat st;
        int status = stat(path,&st);
        if(status == -1) {
            cout << "(FALSE)" << endl;
            return false;
        }
        if(nMode == 0) {
            cout << "(TRUE)" << endl;
            return true;
        }
        if(nMode == 1) {
            if(S_ISREG(st.st_mode) != 0) {
                cout << "(TRUE)" << endl;
                return true;
            } else {
                cout << "(FALSE)" << endl;
                return false;
            }
        }
        if(nMode == 2) {
            if(S_ISDIR(st.st_mode) != 0) {
                cout << "(TRUE)" << endl;
                return true;
            } else {
                cout << "(FALSE)" << endl;
                return false;
            }
        }
    }


};


#endif //RSHELL_CTESTCMD_H
