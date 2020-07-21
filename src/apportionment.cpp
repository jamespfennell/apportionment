
#include <iostream>
#include <vector>
#include <string>
#include <cmath> 
#include <unordered_map>


#include "../include/apportionment.hpp"

using namespace std;
// class IntegerOverRadical();

ApportionmentSession::ApportionmentSession(vector<State> states): stateNameToSeats{}, stateNameToState{}, heap{} {
        for (const auto& state : states) {
            this->stateNameToSeats.insert(make_pair(state.name, 1));
            this->stateNameToState.insert(make_pair(state.name, state));
            this->heap.add(state.name, this->calculatePriorityNumber(state));
        }
    }




long double ApportionmentSession::calculatePriorityNumber(const State& state) {
        long n = stateNameToSeats[state.name] * (1 + stateNameToSeats[state.name]);
        return state.population / sqrt(n);
    }


ApportionedSeat ApportionmentSession::apportionSeat() {
        State state = this->stateNameToState.at(this->heap.pop());
        this->stateNameToSeats[state.name]++;
        long double priorityNumber = this->calculatePriorityNumber(state);
        this->heap.add(state.name, priorityNumber);
        return ApportionedSeat{state, this->stateNameToSeats[state.name], -1,
         priorityNumber};
    }
