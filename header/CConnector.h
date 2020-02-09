#ifndef CS100PROJ_CCONNECTOR_H
#define CS100PROJ_CCONNECTOR_H

#include "CBase.h"

class CConnector : public CBase {
public:
    CBase *leftSideItems;
    CBase *rightSideItems;

    CConnector() {
        leftSideItems = nullptr;
        rightSideItems = nullptr;
    }

    CConnector(CBase *l, CBase *r) {
        leftSideItems = l;
        rightSideItems = r;
    }

    virtual ~CConnector() {
        cleanup();
    }

    virtual void cleanup() {

        if (leftSideItems) {
            delete leftSideItems;
            leftSideItems = nullptr;
        }

        if (rightSideItems) {
            delete rightSideItems;
            rightSideItems = nullptr;
        }
    }


    virtual bool execute() {
        if (leftSideItems) {
            leftSideItems->execute();
        }

        if (rightSideItems) {
            rightSideItems->execute();
        }

        cleanup();
    };


};


#endif //CS100PROJ_CCONNECTOR_H
