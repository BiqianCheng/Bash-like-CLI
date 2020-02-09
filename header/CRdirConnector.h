#ifndef CS100PROJ_CRDIRCONNECTOR_H
#define CS100PROJ_CRDIRCONNECTOR_H


#include "CConnector.h"

class CRdirConnector : public CConnector {

public:

    CRdirConnector() {}

    CRdirConnector(CBase *l, CBase *r) : CConnector(l, r) {}


    bool execute() {
        bool bSuccessful = false;
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
