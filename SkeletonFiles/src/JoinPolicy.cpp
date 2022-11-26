#include "JoinPolicy.h"



Party LastOfferJoinPolicy:: select(vector<Party> offers)
{
    return offers.back();
};

LastOfferJoinPolicy *LastOfferJoinPolicy::duplicate()
{
    return new LastOfferJoinPolicy;
}

Party MandatesJoinPolicy:: select(vector<Party> offers)
{
    int currMaxMandates = 0;
    int PartyMandates;
    Party bestParty = offers.at(0); //just some random party

    for (Party p:offers)
    {
        PartyMandates = p.getMandates();
        if (PartyMandates > currMaxMandates)
        {
            bestParty = p;
            currMaxMandates = PartyMandates;
        }
    }

    return bestParty;
};

MandatesJoinPolicy *MandatesJoinPolicy::duplicate()
{
    return new MandatesJoinPolicy;
}
