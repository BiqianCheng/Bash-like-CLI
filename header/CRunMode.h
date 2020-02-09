#ifndef RSHELL_CRUNMODE_H
#define RSHELL_CRUNMODE_H


class CRunMode {
public:
    static int nMode; // 0 Normal  1 Test (no error message in googletest mode)  2 Quit

public:
    static bool isTestingMode() {
        return (nMode == 1);
    }

    static bool isEnding() {
        return (nMode == 2);
    }

    static void setMode(int n) {
        nMode = n;
    }

};


#endif //RSHELL_CRUNMODE_H
