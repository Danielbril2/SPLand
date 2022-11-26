
#include "Graph.h"

Graph::Graph(vector<Party> vertices, vector<vector<int>> edges) : mVertices(vertices), mEdges(edges) 
{
    // You can change the implementation of the constructor, but not the signature!
}

const int Graph::getMandates(int partyId) const
{
    return mVertices[partyId].getMandates();
}

int Graph::getEdgeWeight(int v1, int v2) const
{
    return mEdges[v1][v2];
}

int Graph::getNumVertices() const
{
    return mVertices.size();
}

const Party &Graph::getParty(int partyId) const
{
    return mVertices[partyId];
}

const Party *Graph::getpartyPointer(int partyId)
{
    return &mVertices[partyId];
}

const vector<int> Graph::getNeigh(int partyId) const
{
    vector<int> res;
    int counter = 0;

    for (int weight: mEdges[partyId]){
        if (weight > 0)
            res.push_back(counter);
        counter++;
    }

    return res;
}

void Graph::setVerticies(vector<Party> parties)
{
    for (Party p: parties)
        mVertices[p.getId()] = p;
}
