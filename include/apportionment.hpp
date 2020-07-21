
#include "heap.hpp"

struct State {
    std::string name;
    long long population;
    long apportionment; // TODO: delete

      bool operator==(const State &other) const
  { return (name == other.name);
  }
};

namespace std {

  template <>
  struct hash<State>
  {
    std::size_t operator()(const State& state) const
    {
        return hash<std::string>()(state.name);
    }
  };

}


struct ApportionedSeat {
    State state;
    long stateSeat;
    long houseSeat;
    long double priorityNumber;
};

class ApportionmentSession {
    std::unordered_map<std::string, long> stateNameToSeats;
    std::unordered_map<std::string, State> stateNameToState;
    Heap<std::string> heap;

    long double calculatePriorityNumber(const State& state);

    public:
    ApportionmentSession(std::vector<State> states);

    ApportionedSeat apportionSeat();
};
