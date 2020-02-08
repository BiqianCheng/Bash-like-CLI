#ifndef CS100PROJ_CANDCONNECTOR_H
#define CS100PROJ_CANDCONNECTOR_H


#include "CConnector.h"

class CAndConnector : public CConnector {

public:
    bool execute(){
        bool bSuccessful;
        if(leftSideItems){
            bSuccessful = leftSideItems->execute();
        }

        if(!bSuccessful)
            return false;

        if(rightSideItems){
            bSuccessful = rightSideItems->execute();
        }
        return bSuccessful;
    };

};


#endif //CS100PROJ_CANDCONNECTOR_H
