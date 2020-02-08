#ifndef CS100PROJ_CRDIRCONNECTOR_H
#define CS100PROJ_CRDIRCONNECTOR_H


#include "CConnector.h"

class CRdirConnector : public CConnector {

public:
    bool execute() {
        bool bSuccessful;
        if (leftSideItems) {
            bSuccessful = leftSideItems->execute();
        }
        if (!bSuccessful)
            return false;

        if (rightSideItems) {
            bSuccessful = rightSideItems->execute();
        }
        return bSuccessful;
    };
};


#endif //CS100PROJ_CRDIRCONNECTOR_H
