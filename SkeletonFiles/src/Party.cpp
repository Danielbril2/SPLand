
#include "Party.h"
#include "Simulation.h"
#include "Agent.h"
#include "JoinPolicy.h"

using std::nullptr_t;

Party::Party(int id, string name, int mandates, JoinPolicy *jp) : mId(id), mName(name), mMandates(mandates), mJoinPolicy(jp), mState(Waiting), timer(3), coalitionOptions(),parties(),isJoin(false)
{
};


Party::~Party() //detructor
{
    if (mJoinPolicy) delete mJoinPolicy;
    mJoinPolicy = nullptr;
}

Party::Party(const Party &p): mId(p.mId), mName(p.mName), mMandates(p.mMandates), mJoinPolicy(p.mJoinPolicy->duplicate()),mState(p.mState),timer(p.timer), coalitionOptions(p.coalitionOptions),parties(p.parties),isJoin(p.isJoin)//copy constructor
{
}

Party::Party(Party &&p): mId(p.mId),mName(p.mName), mMandates(p.mMandates), mJoinPolicy(p.mJoinPolicy->duplicate()),mState(p.mState),timer(p.timer),coalitionOptions(p.coalitionOptions),parties(p.parties),isJoin(p.isJoin) //move constructor
{
    p.mJoinPolicy = nullptr;
    p.coalitionOptions.clear();
    p.parties.clear();
}

Party &Party::operator=(const Party &p) //copy assignemnt operator
{
    if (this != &p)
    {
        if (mJoinPolicy) delete mJoinPolicy;
        if (coalitionOptions.capacity() > 0) coalitionOptions.clear();
        if (parties.capacity() > 0) parties.clear();

        mId = p.mId;
        mMandates = p.mMandates;
        mName = p.mName;
        mJoinPolicy = p.mJoinPolicy->duplicate();
        mState = p.mState;
        timer = p.timer;
        isJoin = p.isJoin;

        for (Party party: p.coalitionOptions)
            coalitionOptions.push_back(party);

        for (int part: p.parties)
            parties.push_back(part);
    }

    return *this;
}

Party &Party::operator=(Party &&p) //move assignemnt operator
{
    if (mJoinPolicy) delete mJoinPolicy;
    if (coalitionOptions.capacity() > 0) coalitionOptions.clear();

    mId = p.mId;
    mMandates = p.mMandates;
    mName = p.mName;
    mJoinPolicy = p.mJoinPolicy->duplicate();
    mState = p.mState;
    timer = p.timer;
    coalitionOptions = p.coalitionOptions;
    parties = p.parties;
    isJoin = p.isJoin;

    p.mJoinPolicy = nullptr;
    p.coalitionOptions.clear();
    p.parties.clear();

    return *this;
}

State Party::getState() const
{
    return mState;
}

const int Party::getId()
{
    return mId;
}

void Party::setState(State state)
{
    mState = state;
}

int Party::getMandates() const
{
    return mMandates;
}

const string & Party::getName() const
{
    return mName;
}

void Party::recieveProposes(Party *p)
{
    coalitionOptions.push_back(*p);
    this->setState(CollectingOffers);
    mState = CollectingOffers;
};

void Party::step(Simulation &s)
{
    if (mState == CollectingOffers)
    {
        timer = timer - 1;
    }
    
    if (timer == 0 && mState != Joined) //need to make a decision
    {
        Party joinedParty = mJoinPolicy->select(coalitionOptions);
        JoinCoalition(s, joinedParty);
    }
}

void Party::JoinCoalition(Simulation &sim, Party &p)
{
    isJoin = true;
    parties.push_back(this->getId());
    parties.push_back(p.getId());
    //sim.acceptPropose(*this, p);
    //tell the agent of the party that we ready to accept its offer
    this->setState(Joined);
}

const vector<Party> Party::getCoalitionOptions() const {return this->coalitionOptions;}

bool Party::getIsJoin()
{
    bool res = isJoin;
    this->isJoin = false;
    return res;
}

vector<int> Party::getParties()
{
    return parties;
}
