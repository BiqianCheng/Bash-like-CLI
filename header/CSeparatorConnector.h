#ifndef CS100PROJ_CSEPARATORCONNECTOR_H
#define CS100PROJ_CSEPARATORCONNECTOR_H

#include "CConnector.h"

class CSeparatorConnector : public CConnector {

public:

    CSeparatorConnector() {}

    CSeparatorConnector(CBase *l, CBase *r) : CConnector(l, r) {}

    bool execute() {
        bool bSuccessful = false;
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
