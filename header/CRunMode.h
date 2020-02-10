#ifndef RSHELL_CRUNMODE_H
#define RSHELL_CRUNMODE_H


class CRunMode {
public:
    //static int nMode; // 0 Normal  1 Test (no error message in googletest mode)  2 Quit
    typedef  enum _runningMode { R_NORMAL, R_TESTING, R_ENDING }  RunningMode;

    static RunningMode nMode;
public:
    static bool isTestingMode() {
        return (nMode == R_TESTING);
    }

    static bool isEnding() {
        return (nMode == R_ENDING);
    }

    static void setMode(RunningMode n) {
        nMode = n;
    }
};


#endif //RSHELL_CRUNMODE_H
