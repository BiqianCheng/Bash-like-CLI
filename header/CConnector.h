#ifndef CS100PROJ_CCONNECTOR_H
#define CS100PROJ_CCONNECTOR_H

#include "CBase.h"

class CConnector : public CBase {
public:
    CBase *leftSideItems;
    CBase *rightSideItems;

    CConnector() {}

    CConnector(CBase *l, CBase *r) {
        leftSideItems = l;
        rightSideItems = r;
    }


    virtual bool execute() {
        if (leftSideItems) {
            leftSideItems->execute();
        } else if (rightSideItems) {
            rightSideItems->execute();
        }
    };



};


#endif //CS100PROJ_CCONNECTOR_H
