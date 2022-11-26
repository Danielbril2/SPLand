#include <iostream>

#include "Simulation.h"

using std::cout;
using std::endl;
using std::string;

Simulation::Simulation(Graph graph, vector<Agent> agents) : mGraph(graph), mAgents(agents), nextId(0), parties()
{
    nextId = findCurrAid() + 1;
    initializeParties();
}

void Simulation::initializeParties()
{
    parties.clear();

    for (int i = 0; i < getGraph().getNumVertices(); i++)
    {
        Party p = getParty(i);
        parties.push_back(p);
    }
}

int Simulation::findCurrAid() const
{
    int currId = 0;
    for (Agent a : mAgents)
    {
        if (a.getId() > currId)
            currId = a.getId();
    }
    return currId;
}

const int Simulation::getAid() //generates the nex id for agents
{
    int res = nextId;
    nextId++;
    return res;
}

void Simulation::step()
{
    vector<Party> temp;
    // step for all parties - decide if they want to join a colition or not
    for (Party p: parties){
        p.step(*this);
        if (p.getIsJoin())
        {
            vector<int> tempParties = p.getParties();
            acceptPropose(tempParties.at(0),tempParties.at(1));
        }
        temp.push_back(p);
    }
    parties = temp;
    mGraph.setVerticies(parties);
    initializeParties(); //updating the parties vector

    // step for all  agents - decide whom they want to make an offer
    //vector<Agent> agentTemp;
    for (Agent a : mAgents){
        if (a.getFather() == nullptr){ //just the original agents can make a "move"
            a.step(*this);
            vector<vector<int>> offers = a.getOffers();
            for (vector<int> offer: offers) //[offeredID, offeringID]
            {
                parties.at(offer.at(0)).recieveProposes(&parties.at(offer.at(1)));
            }

        }
        
    }

}

void Simulation::acceptPropose(int offered, int offering)
{
    //tell the agent of offering that offered ready to join
    vector<Agent> temp;
    for (Agent a: mAgents)
    {
        if (a.getPartyId() == offering) //this is the agent we need to "talk" to
            a.acceptPropose(&parties.at(offered),getAid());

        temp.push_back(a);
    }

    mAgents = temp;
}

bool Simulation::shouldTerminate() const
{
    const vector<vector<int>> allCoalitions = getPartiesByCoalitions();
    int totalMandates = 0;

    // for each coalition checking the sum of mandates
    for (vector<int> coalition : allCoalitions)
    {
        int mandatesSum = 0; // the sum of mandates in a coalition

        for (int partyId : coalition)
            mandatesSum += getParty(partyId).getMandates();

        if (mandatesSum >= 61)
            return true;

        totalMandates += mandatesSum;
    }

    // cout<<"there is no coalition yet ";
    // if the total sum of mandates is 120 it means all parties are in coalitions
    if (totalMandates >= 120)
        return true;

    return false;
}

const Graph &Simulation::getGraph() const
{
    return mGraph;
}

const vector<Agent> &Simulation::getAgents() const
{
    return mAgents;
}

const Party &Simulation::getParty(int partyId) const
{
    return mGraph.getParty(partyId);
}

/// This method returns a "coalition" vector, where each element is a vector of party IDs in the coalition.
/// At the simulation initialization - the result will be [[agent0.partyId], [agent1.partyId], ...]
const vector<vector<int>> Simulation::getPartiesByCoalitions() const
{
    vector<vector<int>> allCoalitions;

    for (Agent hCoalition : mAgents)
    {
        vector<int> parties = hCoalition.getCoalition();

        allCoalitions.push_back(parties);
    }

    return allCoalitions;
}
