
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
    a = b;
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
  struct ReducedForm {
    long long a1;
    long long a2;
    long long b;
    long long c1;
    long long c2;
    long long d;

    ReducedForm(const IntegerOverRadical &ior1, const IntegerOverRadical &ior2)
        : a1{ior1.numerator}, b{ior1.denominatorSquared}, c1{ior2.numerator},
          d{ior2.denominatorSquared} {
      // reduce(b, d);
      // reduce(a1, c1);
      a2 = a1;
      c2 = c1;
      // reduce(a1, b);
      // reduce(a2, b);
      // reduce(c1, d);
      // reduce(c2, d);
    }
  };

public:
  long long numerator;
  long long denominatorSquared;

  // IntegerOverRadical(long long numerator, long long denominatorSquared)
  //   : numerator{numerator}, denominatorSquared{denominatorSquared} {
  // };
  /*
  IntegerOverRadical& operator=(IntegerOverRadical&& other)
{
return *this;
}*/
  operator std::string() const {
    return to_string(numerator) + " / sqrt(" + to_string(denominatorSquared) +
           ")";
  }

  bool operator==(const IntegerOverRadical &other) const {
    /*
     * The implementation here is based on the idea that if a^2 * d = c^2 * d,
     * then by finding all common factors of both sides (which we can do without
     * performing the multiplications) and then removing these from each side
     * will reduce the equation to 1 = 1.
     */
    ReducedForm reducedForm = ReducedForm(*this, other);
    return (not willOverflow(reducedForm.a1, reducedForm.a2, reducedForm.d) and
            not willOverflow(reducedForm.c1, reducedForm.c2, reducedForm.b) and
            reducedForm.a1 * reducedForm.a2 * reducedForm.d == 1 and
            reducedForm.c1 * reducedForm.c2 * reducedForm.b == 1);
  }

  // TODO, can actually make this <=
  bool operator<=(const IntegerOverRadical &other) const {
    ReducedForm reducedForm = ReducedForm(*this, other);
    if (willOverflow(reducedForm.a1, reducedForm.a2, reducedForm.d)) {
      if (willOverflow(reducedForm.c1, reducedForm.c2, reducedForm.b)) {
        cerr << "Failed to use integer arithmetic..." << endl;
        return (static_cast<long double>(reducedForm.a1) / reducedForm.c1) *
                   (static_cast<long double>(reducedForm.a2) /
                    reducedForm.c2) <=
               static_cast<long double>(reducedForm.b) / reducedForm.d;
      } else {
        return false;
      }
    } else {
      if (willOverflow(reducedForm.c1, reducedForm.c2, reducedForm.b)) {
        return true;
      } else {
        return reducedForm.a1 * reducedForm.a2 * reducedForm.d <=
               reducedForm.c1 * reducedForm.c2 * reducedForm.b;
      }
    }
  }

  operator long double() const { 
    return numerator / sqrt(denominatorSquared); 
    }
};

IntegerOverRadical calculatePriorityNumberIOR(const State &state, long seats) {
  IntegerOverRadical ior;

  long long a = state.population;
  return IntegerOverRadical{a, seats * (1 + seats)};
}

// rename calculateCurrentPriorityNumber
long double calculatePriorityNumberLong(const State &state, long seats) {
  long n = seats * (1 + seats);
  return state.population / sqrt(n);
}

template <typename W> class HeapImpl : public ApportionmentSession::Impl {
  long totalSeats;
  std::unordered_map<State, long> stateToSeats;
  Heap<State, W> heap;
  W (*calculatePriorityNumber)(const State &, long);

  // TODO:
public:
  HeapImpl(W (*calculatePriorityNumber)(const State &, long)) {
    this->calculatePriorityNumber = calculatePriorityNumber;
  }

  ~HeapImpl() {}
  void initialize(const vector<State> &states) {
    totalSeats = states.size();
    for (const auto &state : states) {
      this->stateToSeats.insert(make_pair(state, 1));
      this->heap.add(state, calculatePriorityNumber(state, 1));
    }
  }

  const std::unordered_map<State, long> &getCurrentApportionment() const {
    return stateToSeats;
  }

  ApportionedSeat apportionSeat() {
    State state = heap.pop();
    W oldPriorityNumber = calculatePriorityNumber(state, stateToSeats[state]);
    stateToSeats[state]++;
    totalSeats++;
    W priorityNumber = calculatePriorityNumber(state, stateToSeats[state]);
    heap.add(state, priorityNumber);
    return ApportionedSeat{state, stateToSeats[state],  totalSeats,
                           static_cast<long double>(oldPriorityNumber),
                           };
  }
};

ApportionmentSession::ApportionmentSession(const vector<State> &states,
                                           ArithmeticMethod arithmeticMethod) {
  switch (arithmeticMethod) {
  case ArithmeticMethod::EXACT:
    impl = std::unique_ptr<Impl>{
        new HeapImpl<IntegerOverRadical>(&calculatePriorityNumberIOR)};
    break;
  case ArithmeticMethod::CENSUS_BUREAU:
    impl = std::unique_ptr<Impl>{
        new HeapImpl<long double>(&calculatePriorityNumberLong)};
    break;
  }
  impl->initialize(states);
}

unordered_map<State, unordered_map<int, int>>
buildApportionments(const vector<State> &states, const int &minApportionment,
                    const int &maxApportionment,
                    ArithmeticMethod arithmeticMethod) {

  ApportionmentSession session = ApportionmentSession(states, arithmeticMethod);
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