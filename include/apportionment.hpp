
#include "heap.hpp"

struct State {
    string name;
    long long population;
    long apportionment;

    State(string name, long long population) {
        this->name = name;
        this->population = population;
        this->apportionment = 1;
    }
};


struct ApportionedSeat {
    State state;
    long stateSeat;
    long houseSeat;
    long double priorityNumber;
};

class ApportionmentSession {
    unordered_map<string, long> stateNameToSeats;
    unordered_map<string, State> stateNameToState;
    Heap<string> heap;

    long double calculatePriorityNumber(const State& state);

    public:
    ApportionmentSession(vector<State> states);

    ApportionedSeat apportionSeat();
};
