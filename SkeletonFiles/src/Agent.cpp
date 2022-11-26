#include "Agent.h"
#include "Simulation.h"

Agent::Agent(int agentId, int partyId, SelectionPolicy *selectionPolicy) : mAgentId(agentId), mPartyId(partyId), mSelectionPolicy(selectionPolicy), father(nullptr), children(),offers(), currCoalition()
{
}

Agent::Agent(Agent *agent, int partyId, int agentId): mAgentId(agentId), mPartyId(partyId), mSelectionPolicy(agent->mSelectionPolicy->duplicate()), father(agent), children(),offers(), currCoalition()
{  
}

Agent::Agent(const Agent &a): mAgentId(a.mAgentId), mPartyId(a.mPartyId), mSelectionPolicy(a.mSelectionPolicy->duplicate()), father(a.father), children(a.children),offers(a.offers), currCoalition(a.currCoalition) //copy constructor
{
}

Agent::~Agent() //destructor
{
    if (mSelectionPolicy) delete mSelectionPolicy;
    mSelectionPolicy = nullptr;
    if (father) delete father;
    father = nullptr;
}

Agent::Agent(Agent &&a): mAgentId(a.mAgentId),mPartyId(a.mPartyId), mSelectionPolicy(a.mSelectionPolicy->duplicate()),father(a.father), children(a.children), offers(a.offers), currCoalition(a.currCoalition) //move constructor
{
    a.father = nullptr;
    a.mSelectionPolicy = nullptr;
    a.children.clear();
    a.offers.clear();
    a.currCoalition.clear();
}

Agent &Agent::operator=(const Agent &a) //copy assignemnt operator
{
    if (this != &a)
    {
        if (father) delete father;
        if (mSelectionPolicy) delete mSelectionPolicy;
        if (children.capacity() > 0) children.clear();
        if (offers.capacity() > 0) offers.clear();
        if (currCoalition.capacity() > 0) currCoalition.clear();

        mAgentId = a.mAgentId;
        mPartyId = a.mPartyId;
        mSelectionPolicy = a.mSelectionPolicy->duplicate();
        father = a.father;

        for (Agent *agent:a.children)
            children.push_back(agent);

        for (vector<int> offer: a.offers)
            offers.push_back(offer);

        for (int id: a.currCoalition)
            currCoalition.push_back(id);

    }

    return *this;
}

Agent &Agent::operator=(Agent &&a) //move assignemnt operator
{
    if (father) delete father;
    if (mSelectionPolicy) delete mSelectionPolicy;
    if (children.capacity() > 0) children.clear();
    if (offers.capacity() > 0) offers.clear();
    if (currCoalition.capacity() > 0) currCoalition.clear();

    mAgentId = a.mAgentId;
    mPartyId = a.mPartyId;
    mSelectionPolicy = a.mSelectionPolicy->duplicate();
    father = a.father;
    children = a.children;
    offers = a.offers;
    currCoalition = a.currCoalition;

    a.father = nullptr;
    a.mSelectionPolicy = nullptr;
    a.children.clear();
    a.offers.clear();
    a.currCoalition.clear();

    return *this;
}

int Agent::getId() const
{
    return mAgentId;
}

int Agent::getPartyId() const
{
    return mPartyId;
}

Agent *Agent::getFather() const
{
    return this->father;
}

void Agent::step(Simulation &sim)
{
    const Graph* g = &sim.getGraph();

    //create a vector of all parties agent can propose to join
    vector<Party> allPossibilities;

    const vector<int> neigh = g->getNeigh(mPartyId); //all id's of neighbours

    for (int currN: neigh) //for each neighbot check if we can propose to it
    {
        Party neighbor = g->getParty(currN);
        if (neighbor.getState() != Joined)
        {
            bool canOffer = true; //cheking that *we* didnt offer, need to check for all children also
            vector<Party> neighbotOffers = neighbor.getCoalitionOptions();
            for (Party p: neighbotOffers)
                if (p.getId() == mPartyId)
                    canOffer = false;
            if (canOffer)
                allPossibilities.push_back(neighbor);
        }
    };
    
    //select the chosen party from all options and offer her to join
    if (allPossibilities.capacity() > 0){
        Party selectedParty = mSelectionPolicy->select(allPossibilities, g, mPartyId);

        vector<int> temp;
        temp.push_back(selectedParty.getId());
        temp.push_back(mPartyId);
        offers.push_back(temp);
    }

    //run the step for all agents we sent
    this->stepForChildren(sim);

    //TODO: if adding agent from child, need this agent to be from father
}

void Agent::stepForChildren(Simulation &sim)
{
    for (Agent *a: children)
        a->step(sim);
}

vector<vector<int>> Agent::getOffers()
{
    for (Agent *a: children){
        vector<vector<int>> temp = a->getOffers();
        for (vector<int> offer: temp)
            offers.push_back(offer);
    }
    return offers;
}

void Agent::acceptPropose(Party *p, int aId)
{
    //creating new agent that belongs to us, means to our coalition
    Agent *newAgent;
    //the if made to make sure that there is always only one same father to all coaliiton
    if (father == nullptr)
        newAgent = new Agent(this, p->getId(), aId);
    else
        newAgent = new Agent(father, p->getId(), aId);

    children.push_back(newAgent);
}

vector<int> Agent::CreateCoalition()
{
    //this function create a vector of all agents that are members of the same coalition
    //also updating the curr coalition
    vector<int> res;
    res.push_back(mAgentId);
    currCoalition.push_back(mPartyId);
    for (Agent *a: children)
    {
        res.push_back(a->getId());
        currCoalition.push_back(a->getPartyId());
    }

    return res;
}

vector<int> Agent::getCoalition()
{
    this->CreateCoalition(); //updating currCOalition in Create
    return this->currCoalition;
}


vector<Agent*> Agent::getChildren() const
{
    return this->children;
}