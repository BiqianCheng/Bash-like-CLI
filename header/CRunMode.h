#ifndef RSHELL_CRUNMODE_H
#define RSHELL_CRUNMODE_H


class CRunMode {
public:
    static int nMode; // 0 Normal  1 Test (no error message in googletest mode)

public:
    static bool isTestingMode() {
        return (nMode == 1);
    }

};


#endif //RSHELL_CRUNMODE_H
