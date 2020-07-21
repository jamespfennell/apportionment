
#include <cmath>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "../include/apportionment.hpp"

using namespace std;
// class IntegerOverRadical();

ApportionmentSession::ApportionmentSession(vector<State> states)
    : stateToSeats{}, heap{} {
  for (const auto &state : states) {
    this->stateToSeats.insert(make_pair(state, 1));
    this->heap.add(state, this->calculatePriorityNumber(state));
  }
}

// TODO: rename calculateCurrentPriorityNumber
long double ApportionmentSession::calculatePriorityNumber(const State &state) {
  long n = stateToSeats[state] * (1 + stateToSeats[state]);
  return state.population / sqrt(n);
}

const std::unordered_map<State, long> &
ApportionmentSession::getCurrentApportionment() const {
  return stateToSeats;
}

ApportionedSeat ApportionmentSession::apportionSeat() {
  State state = this->heap.pop();
  this->stateToSeats[state]++;
  long double priorityNumber = this->calculatePriorityNumber(state);
  this->heap.add(state, priorityNumber);
  return ApportionedSeat{state, this->stateToSeats[state], -1, priorityNumber};
}

unordered_map<State, unordered_map<int, int>>
buildApportionments(const vector<State> &states, const int &minApportionment,
                    const int &maxApportionment) {

  ApportionmentSession session = ApportionmentSession(states);
  unordered_map<State, unordered_map<int, int>> stateToNumSeatsToApportionment;
  const unordered_map<State, long> &stateToSeats =
      session.getCurrentApportionment();
  for (int i = states.size() + 1; i <= maxApportionment; i++) {
    ApportionedSeat apportionedSeat = session.apportionSeat();
    if (i < minApportionment) {
      continue;
    }
    for (const auto &state : states) {
      stateToNumSeatsToApportionment[state][i] = stateToSeats.at(state);
    }
  }
  return stateToNumSeatsToApportionment;
}