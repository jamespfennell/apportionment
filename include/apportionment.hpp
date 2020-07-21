
#include "heap.hpp"

struct State {
  std::string name;
  long long population;

  bool operator==(const State &other) const { return (name == other.name); }
};

namespace std {
template <> struct hash<State> {
  std::size_t operator()(const State &state) const {
    return hash<std::string>()(state.name);
  }
};
} // namespace std

struct ApportionedSeat {
  State state;
  long stateSeat;
  long houseSeat;
  long double priorityNumber;
};

class ApportionmentSession {
  std::unordered_map<State, long> stateToSeats;
  Heap<State> heap;

  long double calculatePriorityNumber(const State &state);

public:
  ApportionmentSession(std::vector<State> states);

  const std::unordered_map<State, long> &getCurrentApportionment() const;

  ApportionedSeat apportionSeat();
};

std::unordered_map<State, std::unordered_map<int, int>>
buildApportionments(const std::vector<State> &states,
                    const int &minApportionment, const int &maxApportionment);