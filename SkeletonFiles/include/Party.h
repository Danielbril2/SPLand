#pragma once

#include <string>
#include <vector>

using std::string;
using std::vector;

class JoinPolicy;
class Simulation;

enum State
{
    Waiting,
    CollectingOffers,
    Joined
};

class Party
{
public:
    Party(int id, string name, int mandates, JoinPolicy *); 
    virtual ~Party(); //detructor
    Party(const Party &p); //copy constructor
    Party(Party &&p); //move constructor
    Party &operator=(const Party &p); //copy assignment operator
    Party &operator=(Party &&p); //move assignment operator

    State getState() const;
    const int getId();
    void setState(State state);
    int getMandates() const;
    void step(Simulation &s);
    const string &getName() const;
    void recieveProposes(Party *p);
    const std::vector<Party> getCoalitionOptions() const;
    bool getIsJoin();
    vector<int> getParties(); //contains out id and the id of the party we want to join
    
private:
    int mId;
    string mName;
    int mMandates;
    JoinPolicy *mJoinPolicy;
    State mState;
    int timer;
    std::vector<Party> coalitionOptions;
    vector<int> parties;
    bool isJoin;

    void JoinCoalition(Simulation &sim, Party &p);
};
