
#include <iostream>
#include <vector>
#include <string>
#include <cmath> 
#include <unordered_map>

using namespace std;

#include "../include/heap.hpp"
#include "../include/apportionment.hpp"

// class IntegerOverRadical();

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

    long double calculatePriorityNumber(const State& state) {
        long n = stateNameToSeats[state.name] * (1 + stateNameToSeats[state.name]);
        return state.population / sqrt(n);
    }

    public:
    ApportionmentSession(vector<State> states): stateNameToSeats{}, stateNameToState{}, heap{} {
        for (const auto& state : states) {
            this->stateNameToSeats.insert(make_pair(state.name, 1));
            this->stateNameToState.insert(make_pair(state.name, state));
            this->heap.add(state.name, this->calculatePriorityNumber(state));
        }
    }

    ApportionedSeat apportionSeat() {
        State state = this->stateNameToState.at(this->heap.pop());
        this->stateNameToSeats[state.name]++;
        long double priorityNumber = this->calculatePriorityNumber(state);
        this->heap.add(state.name, priorityNumber);
        return ApportionedSeat{state, -1, this->stateNameToSeats[state.name], priorityNumber};
    }
};
