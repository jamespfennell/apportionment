#include <iostream>
#include <unordered_map>
#include <fstream>

#include "../include/heap.hpp"
#include "../include/apportionment.hpp"

long double identity(int i) {
    return i;
}

int main()
{
    ifstream rfile;
    rfile.open("census2010.csv");
    vector<shared_ptr<State>> states = getStatesFromCSV(rfile, true);
    rfile.close();

    vector<State> statesRaw = vector<State>{};
    for (auto statePtr : states) {
        statesRaw.push_back(*statePtr);
    }

    vector<int> nums = {1, 2, 30, 4, 9};
    unique_ptr<MaxHeap<int>> m = unique_ptr<MaxHeap<int>>{new MaxHeap<int>(identity, nums)};
    (*m).pop();
    (*m).add(26);
    MaxHeap<int> m2 = MaxHeap<int>(identity, nums);
    m2.pop();
    m2.add(65);

    unordered_map<string,long> stateNameToApportionment = unordered_map<string,long>{};
    for (auto state : statesRaw) {
        stateNameToApportionment[state.name] = 1;
    }
    
    // This works:
    // states = getStates(cin);

    unique_ptr<MaxHeap<shared_ptr<State>>> maxHeap = unique_ptr<MaxHeap<shared_ptr<State>>>{new MaxHeap<shared_ptr<State>>(calculatePriority, states)};

    for (int i=0; i<435 - 50; i++) {
        shared_ptr<State> highestPriorityState = maxHeap->pop();
        (*highestPriorityState).apportionment++;
        maxHeap->add(highestPriorityState);
    }
    /*
    for (int i=0; i<435 - 50; i++) {
        shared_ptr<State> highestPriorityState = getHighestPriorityState(states);
        // cout << "One seat to " << highestPriorityState->name << endl;
        (*highestPriorityState).apportionment++;
    }
    */

    cout << "entity,apportionment_435" << endl;
    for (auto&& statePtr : states)
    {
        State state = *statePtr;
        cout << state.name << "," << state.apportionment << endl;
    }
    cout << endl;
}