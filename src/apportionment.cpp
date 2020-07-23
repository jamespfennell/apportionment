
#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "../include/apportionment.hpp"
#include "../include/heap.hpp"
using namespace std;

bool willOverflow(long long a, long long b) {
  return a >= numeric_limits<long long>::max() / b;
}

bool willOverflow(long long a, long long b, long long c) {
  return willOverflow(a, b) or willOverflow(a * b, c);
}

long long gcd(long long a, long long b) {
  long long c;
  while (b != 0) {
    c = a % b;
    a = a / b;
    b = c;
  }
  return a;
}

void reduce(long long &a, long long &b) {
  long long g = gcd(a, b);
  a /= g;
  b /= g;
}

class IntegerOverRadical {
  long long numerator;
  long long denominatorSquared;

public:
  bool operator=(const IntegerOverRadical &other) const {
    long long a1 = this->numerator;
    long long b = this->denominatorSquared;
    long long c1 = other.numerator;
    long long d = other.denominatorSquared;
    reduce(b, d);
    reduce(a1, c1);
    long long a2 = a1;
    long long c2 = c1;
    reduce(a1, b);
    reduce(a2, b);
    reduce(c1, d);
    reduce(c2, d);
    return (not willOverflow(a1, a2, d) and not willOverflow(c1, c2, b) and
            a1 * a2 * d == 1 and c1 * c2 * b == 1);
  }

  bool operator<(const IntegerOverRadical &other) const {
    long long a = this->numerator;
    long long b = this->denominatorSquared;
    long long c = other.numerator;
    long long d = other.denominatorSquared;
    reduce(b, d);
    if (willOverflow(a, a, d)) {
      if (willOverflow(c, c, d)) {
        // TODO: this incorrectly uses integer division
        return (static_cast<long double>(a) / c) *
                   (static_cast<long double>(a) / c) <
               static_cast<long double>(b) / d;
      } else {
        return true;
      }
    } else {
      if (willOverflow(c, c, d)) {
        return false;
      } else {
        return a * a * d < c * c * b;
      }
    }
  }
};

template <typename W> class HeapImpl : public ApportionmentSession::Impl {
  std::unordered_map<State, long> stateToSeats;
  Heap<State, W> heap;

  ~HeapImpl() {}

  // TODO: rename calculateCurrentPriorityNumber
  long double calculatePriorityNumber(const State &state) const {
    long n = stateToSeats.at(state) * (1 + stateToSeats.at(state));
    return state.population / sqrt(n);
  }

public:
  void initialize(const vector<State> &states) {
    for (const auto &state : states) {
      this->stateToSeats.insert(make_pair(state, 1));
      this->heap.add(state, this->calculatePriorityNumber(state));
    }
  }

  const std::unordered_map<State, long> &getCurrentApportionment() const {
    return stateToSeats;
  }

  ApportionedSeat apportionSeat() {
    State state = this->heap.pop();
    this->stateToSeats[state]++;
    long double priorityNumber = this->calculatePriorityNumber(state);
    this->heap.add(state, priorityNumber);
    return ApportionedSeat{state, this->stateToSeats[state], -1,
                           priorityNumber};
  }
};

ApportionmentSession::ApportionmentSession(const vector<State> &states) {
  impl = std::unique_ptr<Impl>{new HeapImpl<long double>{}};
  impl->initialize(states);
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