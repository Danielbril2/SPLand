#pragma once
#include <vector>

#include "Party.h"

using std::vector;

class Graph
{
public:
    Graph(vector<Party> vertices, vector<vector<int>> edges);
    const int getMandates(int partyId) const;
    int getEdgeWeight(int v1, int v2) const;
    int getNumVertices() const;
    const Party &getParty(int partyId) const;
    const vector<int> getNeigh(int partyId) const;
    const Party *getpartyPointer(int partyId);
    void setVerticies(vector<Party> parties);

private:
    vector<Party> mVertices;
    vector<vector<int>> mEdges;
};
