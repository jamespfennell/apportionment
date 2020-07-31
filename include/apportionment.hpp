#include <vector>
#include <string>
#include <unordered_map>

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
}

struct ApportionedSeat {
  State state;
  long stateSeat;
  long houseSeat;
  long double priorityNumber;
  std::string priorityNumberString;
};

enum class ArithmeticMethod {
  EXACT, CENSUS_BUREAU
};

class ApportionmentSession {
public:
  class Impl {
  public:
    virtual ~Impl(){};
    void virtual initialize(const std::vector<State> &states) = 0;
    const std::unordered_map<State, long> virtual &
    getCurrentApportionment() const = 0;
    ApportionedSeat virtual apportionSeat() = 0;
  };

  // TODO: enum to select the implemntation?
  ApportionmentSession(const std::vector<State> &states, ArithmeticMethod arithmeticMethod);

  const std::unordered_map<State, long> &getCurrentApportionment() const {
    return (*impl).getCurrentApportionment();
  };

  ApportionedSeat apportionSeat() { return (*impl).apportionSeat(); }

private:
  std::unique_ptr<Impl> impl;
};

std::unordered_map<State, std::unordered_map<int, int>>
buildApportionments(const std::vector<State> &states,
                    const int &minApportionment, 
                    const int &maxApportionment, 
                    ArithmeticMethod arithmeticMethod);