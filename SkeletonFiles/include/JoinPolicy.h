#pragma once
#include <vector>
#include "Party.h"
using std::vector;

class JoinPolicy {
    public: 
        virtual Party select(vector<Party> offers)=0;
        virtual JoinPolicy* duplicate()=0;
        virtual ~JoinPolicy()=default;
};

class MandatesJoinPolicy : public JoinPolicy {
    public: 
        virtual Party select(vector<Party> offers);
        MandatesJoinPolicy *duplicate() override;
};

class LastOfferJoinPolicy : public JoinPolicy {
    public: 
        virtual Party select(vector<Party> offers);
        LastOfferJoinPolicy *duplicate() override;
};