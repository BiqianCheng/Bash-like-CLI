#ifndef CS100PROJ_CSEPARATORCONNECTOR_H
#define CS100PROJ_CSEPARATORCONNECTOR_H

#include "CConnector.h"

class CSeparatorConnector: public CConnector {

public:
    bool execute() {
        bool bSuccessful;
        if (leftSideItems) {
            bSuccessful = leftSideItems->execute();
        }

        if (rightSideItems) {
            bSuccessful = rightSideItems->execute();
        }
        return bSuccessful;
    };
};

#endif //CS100PROJ_CSEPARATORCONNECTOR_H
