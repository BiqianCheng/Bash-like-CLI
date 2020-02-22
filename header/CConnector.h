#ifndef CS100PROJ_CCONNECTOR_H
#define CS100PROJ_CCONNECTOR_H

#include "CBase.h"
#include "CCommand.h"

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
        //cleanup();
    }

    static void cleanIt(CBase *pBase) {
        CConnector *pConnector = nullptr;
        CCommand *pCommand = nullptr;

        if(!pBase)
            return;
        
        pConnector = dynamic_cast<CConnector *>(pBase);
        if (pConnector) {
            CBase *pL = pConnector->leftSideItems;
            if (pL) {
                CConnector::cleanIt(pL);
                pL = nullptr;
            }

            CBase *pR = pConnector->rightSideItems;
            if (pR) {
                CConnector::cleanIt(pR);
                pR = nullptr;
            }
            
            delete pConnector;
            pBase = nullptr;            
        } else {
            pCommand = dynamic_cast<CCommand *>(pBase);
            if (pCommand) {
                delete pCommand;
                pBase = nullptr;
            }
            else if (pBase) {
                delete pBase;
                pBase = nullptr;
            }
        }        
    }

    virtual void cleanup() {
        if (leftSideItems) {
            CConnector::cleanIt(leftSideItems);
            leftSideItems = nullptr;
        }

        if (rightSideItems) {
            CConnector::cleanIt(rightSideItems);
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

        return true;
    };
};


#endif //CS100PROJ_CCONNECTOR_H
