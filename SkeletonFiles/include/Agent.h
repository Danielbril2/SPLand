#pragma once

#include "Graph.h"
#include "SelectionPolicy.h"

using std::vector;
class Simulation;
class SelectionPolicy;

class Agent
{
public:
    Agent(int agentId, int partyId, SelectionPolicy *selectionPolicy);
    Agent(Agent* agent, int partyId, int agentId); //initialize with father
    Agent(const Agent &a); //copy constructor
    virtual ~Agent(); //destructor
    Agent(Agent &&a); //move constructor
    Agent &operator=(const Agent &a); //copy assignemnt operator
    Agent &operator=(Agent &&a); //move assignment operator

    int getPartyId() const;
    int getId() const;
    void step(Simulation &sim);
    void acceptPropose(Party *p, int aId); //cloning agent and sending him to the party
    vector<int> CreateCoalition();
    vector<vector<int>> getOffers(); //type: [[the offerd partyID, offeringPartyId]]
    vector<int> getCoalition();
    Agent *getFather() const;
    vector<Agent*> getChildren() const;
    

private:
    int mAgentId;
    int mPartyId;
    SelectionPolicy *mSelectionPolicy;
    Agent *father; //means from what agent they were sent
    vector<Agent*> children; //what agents we sent
    vector<vector<int>> offers;
    vector<int> currCoalition;

    void stepForChildren(Simulation &);
};

