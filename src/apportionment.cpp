
#include <iostream>
#include <vector>
#include <string>
#include <cmath> 
using namespace std;

#include "../include/apportionment.hpp"


long double calculatePriority(shared_ptr<State> state) {
    long double population = state -> population;
    cout << "calculationg ..." << population << endl;
    long double d = state->apportionment * (state->apportionment + 1);
    cout << "sqrt " << sqrt(d) << endl;
    long double a  = ( population ) / sqrt(d);
    cout << a << "return" << a * sqrt(d) << endl;
    return a;
}

shared_ptr<State> getHighestPriorityState(const vector<shared_ptr<State>>& states) {
    shared_ptr<State> highestPriorityState;
    long double highestPriority = 0;
    for (auto&& state : states) {
        long double thisPriority = calculatePriority(state);
        if (highestPriority < thisPriority) {
            highestPriorityState = state;
            highestPriority = thisPriority;
        }
    }
    return highestPriorityState;
}

vector<shared_ptr<State>> getStatesFromCSV(istream& inputStream, bool skipFirstLine) {
    vector<shared_ptr<State>> states;
    string line;
    if (skipFirstLine) {
        getline(inputStream, line);
    }
    while (std::getline(inputStream, line)) {
        long last_comma = line.find_last_of(",");
        string name = line.substr(0, last_comma);
        string populationString = line.substr(last_comma + 1);
        long long population = stoll(populationString);  // TODO: what if there's an error here?
        states.push_back(shared_ptr<State>{new State(name, population)});
    }
    return states;
}
