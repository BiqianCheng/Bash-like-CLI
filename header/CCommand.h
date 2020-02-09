#ifndef CS100PROJ_CCOMMAND_H
#define CS100PROJ_CCOMMAND_H

#include "CBase.h"
#include "CRunMode.h"
#include "CParser.h"

#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>

#include <cstring>

#include <vector>
#include <string>
#include <fstream>


using namespace std;

class CCommand : public CBase {
public:
    vector<string> vecToken;
    vector<string> vecAnother;

    string strInData;
    string strOutData;

    string strInFileName;
    string strOutFileName;

    unsigned char advMode; // 0-none 1-in 2-out 3-in and out 4-pipe
    unsigned char skipIt;

    CCommand() {
        advMode = 0;
        skipIt = 0;
    }

    CCommand(const char *pFullLine) {
        CParser::ParserLineToVector(pFullLine, vecToken);
        advMode = 0;
        skipIt = 0;
    }

    virtual ~CCommand() {

    }

    void setRdirInFileName(string &s) {
        strInFileName = s;
        advMode = advMode | 1;
    }

    void doRdirIn() {
//        string wstr;
//        ifstream wfin;
//        wfin.open(strInFileName);
//
//        while (getline(wfin, wstr)) {
//            strInData += wstr;
//        }
//
//        wfin.close();

    }

    void setRdirOutFileName(string &s) {
        strOutFileName = s;
        advMode = advMode | 2;
    }

    void doRdirOut() {

        ofstream wfout;
        wfout.open(strOutFileName);
        wfout << strOutData;
        wfout.close();
    }

    bool execute() {
        int nRet = 0;
        bool bResult = true;

        if (skipIt) {
            return true;
        }

        if (advMode != 4) {

            int size = vecToken.size();
            if (size) {

                char **argv = nullptr;

                argv = CParser::vectorToArgv(vecToken);
                //argv = CParser::vectorToArgvWithExtra(vecToken,0, vecExtra);

                if (!strcmp(argv[0], "exit")) {
                    CRunMode::setMode(2);
                    CParser::cleanUpArgv(argv, size);
                    return true;
                }

                if (CRunMode::isTestingMode() || advMode) {
                    int fd[2] = {0, 0};
                    int ret = pipe(fd);
                    runCommand(argv, fd[0], fd[1], nRet);
                } else {
                    runCommand(argv, 0, 1, nRet);
                }

                CParser::cleanUpArgv(argv, size);

                if (nRet == 0)
                    bResult = true;
                else
                    bResult = false;
            }
        } else {

            int fd[2] = {0, 0};
            int ret = pipe(fd);

            int size_A = vecToken.size();
            int size_B = vecAnother.size();
            if (size_A && size_B) {

                char **argv_A = nullptr;
                char **argv_B = nullptr;

                argv_A = CParser::vectorToArgv(vecToken);
                argv_B = CParser::vectorToArgv(vecAnother);

                if (!strcmp(argv_A[0], "exit")) {
                    CRunMode::setMode(2);
                    CParser::cleanUpArgv(argv_A, size_A);
                    CParser::cleanUpArgv(argv_B, size_B);
                    return true;
                }

                if (!strcmp(argv_B[0], "exit")) {
                    CRunMode::setMode(2);
                    CParser::cleanUpArgv(argv_A, size_A);
                    CParser::cleanUpArgv(argv_B, size_B);
                    return true;
                }


                runMultiCommand(argv_A,argv_B,fd[0], fd[1], nRet );

                CParser::cleanUpArgv(argv_A, size_A);
                CParser::cleanUpArgv(argv_B, size_B);

                if (nRet == 0)
                    bResult = true;
                else
                    bResult = false;

            }


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

            if (advMode & 0x01) {
                //doRdirIn();
                int fdNewIn = open(strInFileName.c_str(), O_RDONLY);
                ret = dup2(fdNewIn, STDIN_FILENO);
                close(fdIn);
                fdIn = fdNewIn;
            } else {
                ret = dup2(fdIn, STDIN_FILENO);
                if (ret == -1) {
#ifdef  _MY_DEBUG
                    perror("dup2 error STDIN_FILENO");
#endif
                }
                close(fdIn);
            }

            ret = dup2(fdOut, STDOUT_FILENO);
            if (ret == -1) {
#ifdef  _MY_DEBUG
                perror("dup2 error STDOUT_FILENO");
#endif
            }
            close(fdOut);

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

            close(fdOut);

            char temp[1024] = {0};
            int n = 0;
            do {
                memset(temp, 0, 1024);
                n = read(fdIn, temp, 1024);
                if (n) {
                    strOutData += temp;
                }
            } while (n);

            if (advMode & 0x02) {
                doRdirOut();
            } else {
                if (!CRunMode::isTestingMode()) {
                    cout << strOutData;
                }
            }

            if (WEXITSTATUS(status) == 0) {
                //exit(EXIT_SUCCESS);
                nRet = 0;

            } else {
                //exit(EXIT_FAILURE);
                //perror("waitpid error");
                nRet = __LINE__;
            }
        }
    }

    void runMultiCommand(char **argv_A, char **argv_B, int fdIn, int fdOut, int &nRet) {
        pid_t cpid = 0;
        int ret = 0;
        int status = 0;

        if (fork() == 0)            //first child used to output
        {
            close(STDOUT_FILENO);  //closing stdout
            dup(fdOut);         //replacing stdout with pipe write 
            close(fdIn);       //closing pipe read
            close(fdOut);

            execvp(argv_A[0], argv_A);
            //perror("execvp of ls failed");
            exit(1);
        }
        
        if (fork() == 0)            //second child used to input
        {
            close(STDIN_FILENO);   //closing stdin
            dup(fdIn);         //replacing stdin with pipe read
            close(fdOut);       //closing pipe write
            close(fdIn);

            execvp(argv_B[0], argv_B);
            //perror("execvp of wc failed");
            exit(1);
        }

        if(1)
        {
            close(fdIn);
            close(fdOut);

            wait(0);
            wait(0);

            nRet = 0;

            /*
            char temp[1024] = {0};
            int n = 0;
            do {
                memset(temp, 0, 1024);
                n = read(fdIn, temp, 1024);
                if (n) {
                    strOutData += temp;
                }
            } while (n);

            if (advMode & 0x02) {
                doRdirOut();
            } else {
                if (!CRunMode::isTestingMode()) {
                    cout << strOutData;
                }
            }

            if (WEXITSTATUS(status) == 0) {
                //exit(EXIT_SUCCESS);
                nRet = 0;

            } else {
                //exit(EXIT_FAILURE);
                //perror("waitpid error");
                nRet = __LINE__;
            }
             */
        }
    }

};


#endif //CS100PROJ_CCOMMAND_H
