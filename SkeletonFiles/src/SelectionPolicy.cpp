
#include "SelectionPolicy.h"

Party MandatesSelectionPolicy::select(vector<Party> options, const Graph* g, int currPartyId)
{
    Party bestParty = options.at(0); //random party

    for (Party p:options)
    {
        if (p.getMandates() > bestParty.getMandates())
            bestParty = p;
    }

    return bestParty;;
};

SelectionPolicy *MandatesSelectionPolicy::duplicate()
{
    return new MandatesSelectionPolicy;
}


Party EdgeWeightSelectionPolicy::select(vector<Party> options, const Graph* g, int currPartyId)
{
    Party bestParty = options.at(0); //random party
    int currMaxEdgeWait = 0;

    for (Party p: options)
    {
        int currEdgeW = g->getEdgeWeight(p.getId(),currPartyId);
        if (currEdgeW > currMaxEdgeWait){
            bestParty = p;
            currMaxEdgeWait = currEdgeW;
        }
    }

    return bestParty;
};

SelectionPolicy *EdgeWeightSelectionPolicy::duplicate()
{
    return new EdgeWeightSelectionPolicy;
}

