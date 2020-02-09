#ifndef CS100PROJ_CPIPECONNECTOR_H
#define CS100PROJ_CPIPECONNECTOR_H


#include "CConnector.h"

class CPipeConnector : public CConnector {

public:

    CPipeConnector() {}

    CPipeConnector(CBase *l, CBase *r) : CConnector(l, r) {}

    bool execute() {
        bool bSuccessful = false;

        if(leftSideItems && rightSideItems){

            CCommand * pCmd_A = dynamic_cast< CCommand * >(leftSideItems);
            CCommand * pCmd_B = dynamic_cast< CCommand * >(rightSideItems);
            if(pCmd_A && pCmd_B) {

                pCmd_A->vecAnother = pCmd_B->vecToken;
                pCmd_A->advMode = 4;

                pCmd_B->skipIt = true;

                bSuccessful = pCmd_A->execute();
            }
        }

        return bSuccessful;
    };
};


#endif //CS100PROJ_CPIPECONNECTOR_H
