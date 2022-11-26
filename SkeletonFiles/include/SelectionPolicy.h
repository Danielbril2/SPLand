#pragma once
#include <vector>
#include "Party.h"
#include "Graph.h"

using std::vector;

class SelectionPolicy { 
    public:
        virtual Party select(vector<Party> options,const Graph* g, int currPartyId)=0;
        virtual SelectionPolicy *duplicate()=0;
        virtual ~SelectionPolicy()= default;
};

class MandatesSelectionPolicy: public SelectionPolicy{ 
    public:
        virtual Party select(vector<Party> options, const Graph* g, int currPartyId);
        SelectionPolicy *duplicate() override;
};

class EdgeWeightSelectionPolicy: public SelectionPolicy{ 
    public: 
        virtual Party select(vector<Party> options, const Graph* g, int currPartyId);
        SelectionPolicy *duplicate() override;
};