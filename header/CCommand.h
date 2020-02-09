#ifndef CS100PROJ_CCOMMAND_H
#define CS100PROJ_CCOMMAND_H

#include "CBase.h"
#include "CRunMode.h"

#include <cstdlib>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <cstring>

#include <vector>
#include <string>

using namespace std;

class CCommand : public CBase {
public:
    vector<string> vecToken;

    CCommand() {}

    CCommand(const char *pFullLine) {

        string inputString = pFullLine;
        stringstream stringStream(inputString);
        string line;
        vector<string> wordVector;
        string token;

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
        vecToken = wordVector;
    }

    virtual ~CCommand() {

    }

    bool execute() {
        int nRet = 0;
        bool bResult = true;
        int size = vecToken.size();
        if (size) {

            char **argv = nullptr;

            argv = new char *[size + 1];
            argv[size] = nullptr;

            for (int i = 0; i < size; ++i) {

                string itStr = vecToken[i];
                argv[i] = new char[itStr.length() + 1];
                strcpy(argv[i], itStr.c_str());
            }

            if (!strcmp(argv[0], "exit")) {
                //exit(1);
                CRunMode::setMode(2);

                // clean up
                for (int i = 0; i < size; ++i) {
                    delete[] argv[i];
                }
                delete[] argv;

                return true;
            }

            if (CRunMode::isTestingMode()) {
                int fd[2] = {0, 0};
                int ret = pipe(fd);
                runCommand(argv, fd[0], fd[1], nRet);
            } else {
                runCommand(argv, 0, 1, nRet);
            }

#ifdef  _MY_DEBUG
            cout << "nRet:"<<nRet<< endl;
#endif
            if (nRet == 0)
                bResult = true;
            else
                bResult = false;

            for (int i = 0; i < size; ++i) {
                delete[] argv[i];
            }

            delete[] argv;
        }
        return bResult;
    }

    void runCommand(char **argv, int fdIn, int fdOut, int &nRet) {
        pid_t cpid = 0;
        int ret = 0;
        int status = 0;

        cpid = fork();

        if (cpid == -1) {
            //throw CException("Fork Error!");
            perror("fork error");
            exit(EXIT_FAILURE);
        } else if (cpid == 0) {  // Child process

#ifdef  _MY_DEBUG
            cout << "Child's parent's PID is " << (long) getpid() << endl;
#endif
            ret = dup2(fdIn, STDIN_FILENO);
            if (ret == -1) {
#ifdef  _MY_DEBUG
                perror("dup2 error STDIN_FILENO");
#endif
            }

            if (CRunMode::isTestingMode()) {
                close(fdIn);
            }

            ret = dup2(fdOut, STDOUT_FILENO);
            if (ret == -1) {
#ifdef  _MY_DEBUG
                perror("dup2 error STDOUT_FILENO");
#endif
            }

            if (CRunMode::isTestingMode()) {
                close(fdOut);
            }

#ifdef  _MY_DEBUG
            cout << "execvp " << argv[0] << "  " << argv[1] << endl;
#endif
            ret = execvp(argv[0], argv);
            if (ret == -1) {
//#ifdef  _MY_DEBUG
//            perror("execvp error!!!");
//#endif
                if (!CRunMode::isTestingMode()) {
                    string strErrMsg = "Executing ";
                    strErrMsg += argv[0];
                    strErrMsg += " error";
                    perror(strErrMsg.c_str());
                }

                nRet = __LINE__;
            } else {
                nRet = 0;
            }

            exit(nRet);

        } else {
#ifdef  _MY_DEBUG
            cout << "Parent PID is " << cpid << endl;
#endif
            ret = waitpid(cpid, &status, WUNTRACED | WCONTINUED);
            //ret = wait(&status);
            if (ret == -1) {
#ifdef  _MY_DEBUG
                perror("waitpid error");
#endif
            }

            if (WEXITSTATUS(status) == 0) {
                //exit(EXIT_SUCCESS);
                nRet = 0;

//                cout << "waitpid done"<<endl;

            } else {
                //exit(EXIT_FAILURE);

                //perror("waitpid error");
                nRet = __LINE__;
            }
        }
    }
};


#endif //CS100PROJ_CCOMMAND_H
