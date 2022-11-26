#pragma once

#include <vector>

#include "Agent.h"

using std::string;
using std::vector;

class Simulation
{
public:
    Simulation(Graph g, vector<Agent> agents);

    void step();
    bool shouldTerminate() const;

    const Graph &getGraph()const ;
    const vector<Agent> &getAgents() const;
    const Party &getParty(int partyId) const;
    const vector<vector<int>> getPartiesByCoalitions() const;
    const int getAid();
    void acceptPropose(int offered, int offering);

private:
    Graph mGraph;
    vector<Agent> mAgents;
    int nextId;
    vector<Party> parties;

    void initializeParties();
    int findCurrAid() const; //the next id we can give to a new agent
};

