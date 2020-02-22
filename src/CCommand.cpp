
#include "../header/CCommand.h"
#include "../header/CTestCmd.h"

CCommand::CCommand() {
    advMode = 0;
    skipIt = 0;
}

CCommand::CCommand(const char *pFullLine) {
    CParser::parserLineToVector(pFullLine, vecToken);
    advMode = 0;
    skipIt = 0;
}

CCommand::CCommand(vector<string> & v ) {
    vecToken = v;
    advMode = 0;
    skipIt = 0;
}

CCommand::~CCommand() {

}

void CCommand::setRdirInFileName(string &s) {
    strInFileName = s;
    advMode = advMode | 1;
}

int CCommand::doRdirIn() {
//        string wstr;
//        ifstream wfin;
//        wfin.open(strInFileName);
//
//        while (getline(wfin, wstr)) {
//            strInData += wstr;
//        }
//
//        wfin.close();

    int fd = open(strInFileName.c_str(), O_RDONLY);
    return fd;
}

void CCommand::setRdirOutFileName(string &s) {
    strOutFileName = s;
    advMode = advMode | 2;
}

void CCommand::doRdirOut() {

    ofstream wfout;
    wfout.open(strOutFileName);
    wfout << strOutData;
    wfout.close();
}

bool CCommand::execute() {
    int nRet = 0;
    bool bResult = true;

    if (skipIt)
        return true;

    if (advMode != 4) {

        int size = vecToken.size();
        if (size) {

            char **argv = nullptr;

            argv = CParser::vectorToArgv(vecToken);

            if (!strcmp(argv[0], "exit")) {
                CParser::cleanUpArgv(argv, size);
                CRunMode::setMode(CRunMode::R_ENDING);
                return true;
            }
            else if (!strcmp(argv[0], "test")) {

                CTestCmd * pTestCmd = new CTestCmd(this->vecToken);
                bResult = pTestCmd->execute();

                delete pTestCmd;

                CParser::cleanUpArgv(argv, size);
                return bResult;
            }

//                if (!CRunMode::isTestingMode()) {
//                    runNormalCommand(argv, 0, 1, nRet);
//                }
//                else
            {
                int fd[2] = {0, 0};
                pipe(fd);
                runNormalCommand(argv, fd[0], fd[1], nRet);
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

                CParser::cleanUpArgv(argv_A, size_A);
                CParser::cleanUpArgv(argv_B, size_B);
                CRunMode::setMode(CRunMode::R_ENDING);
                return true;
            }

            if (!strcmp(argv_B[0], "exit")) {

                CParser::cleanUpArgv(argv_A, size_A);
                CParser::cleanUpArgv(argv_B, size_B);
                CRunMode::setMode(CRunMode::R_ENDING);
                return true;
            }

            runMultiCommand(argv_A, argv_B, fd[0], fd[1], nRet);

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

void CCommand::runNormalCommand(char **argv, int fdIn, int fdOut, int &nRet) {
    pid_t cpid = 0;
    int ret = 0;
    int status = 0;
    int fdNewIn = 0;


    //cout << "Run "<< argv[0]<<endl;


    cpid = fork();

    if (cpid == -1) {
        perror("fork error");
        exit(EXIT_FAILURE);
    }

    if (cpid == 0) {  // Child process

        if (advMode & 0x01) {
            //fdNewIn = doRdirIn();
            fdNewIn = open(strInFileName.c_str(), O_RDONLY);
            dup2(fdNewIn, STDIN_FILENO);
            close(fdIn);
        } else {
            close(STDIN_FILENO);
            close(fdIn);
        }

        if (CRunMode::isTestingMode())
            close(2);

        dup2(fdOut, STDOUT_FILENO);
        //close(STDOUT_FILENO);

        ret = execvp(argv[0], argv);
        if (ret == -1) {
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

        close(fdOut);
        exit(nRet);
    } else {
        ret = waitpid(cpid, &status, WUNTRACED | WCONTINUED);

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
                cout << strOutData<<std::flush;
                strOutData = "";
            }
        }

        if (WEXITSTATUS(status) == 0) {
            nRet = 0;

        } else {
            nRet = __LINE__;
        }
    }
}

void CCommand::runMultiCommand(char **argv_A, char **argv_B, int fdIn, int fdOut, int &nRet) {
    pid_t cpid = 0;
    int ret = 0;
    int status = 0;

    if (fork() == 0) {           //first child used to output
        close(STDOUT_FILENO);  //closing stdout
        dup(fdOut);         //replacing stdout with pipe write
        close(fdIn);       //closing pipe read
        close(fdOut);

        execvp(argv_A[0], argv_A);
        //perror("execvp of first one failed");
        exit(1);
    }

    if (fork() == 0) {          //second child used to input
        close(STDIN_FILENO);   //closing stdin
        dup(fdIn);         //replacing stdin with pipe read
        close(fdOut);       //closing pipe write
        close(fdIn);

        execvp(argv_B[0], argv_B);
        //perror("execvp of second one failed");
        exit(1);
    }

    if (1) {
        close(fdIn);
        close(fdOut);

        wait(0);
        wait(0);

        nRet = 0;
    }
}