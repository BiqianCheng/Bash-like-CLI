#ifndef CS100PROJ_CRDIRCONNECTOR_H
#define CS100PROJ_CRDIRCONNECTOR_H


#include "CConnector.h"

class CRdirConnector : public CConnector {

public:
    CRdirConnector() {}

    CRdirConnector(CBase *l, CBase *r) : CConnector(l, r) {}


};


class CRdirInConnector : public CRdirConnector {
public:
    CRdirInConnector() {}

    bool execute() {
        bool bSuccessful = false;


        if(leftSideItems && rightSideItems){

            CCommand * pCmd = dynamic_cast< CCommand * >(leftSideItems);
            CCommand * pParameters = dynamic_cast< CCommand * >(rightSideItems);
            if(pCmd && pParameters) {

                string strFileName = pParameters->vecToken[0];
                pCmd->setRdirInFileName(strFileName);

                pParameters->skipIt = true;

                //CConnector::cleanIt(pParameters);

                pCmd->execute();
            }
        }

        return bSuccessful;
    };
};

class CRdirOutConnector : public CRdirConnector {
public:
    CRdirOutConnector() {}

    bool execute() {
        bool bSuccessful = false;

        if(leftSideItems && rightSideItems){

            CCommand * pCmd = dynamic_cast< CCommand * >(leftSideItems);
            CCommand * pParameters = dynamic_cast< CCommand * >(rightSideItems);
            if(pCmd && pParameters) {

                string strFileName = pParameters->vecToken[0];
                pCmd->setRdirOutFileName(strFileName);

                pParameters->skipIt = true;

                //CConnector::cleanIt(pParameters);

                pCmd->execute();
            }
        }
        return bSuccessful;
    };
};


#endif //CS100PROJ_CRDIRCONNECTOR_H
