#include <iostream>
#include <vector>
#include <string>
#include <cmath> 
#include <fstream>
using namespace std;

struct State {
    string name;
    long long population;
    long apportionment;

    State(string name, long long population) {
        this->name = name;
        this->population = population;
        this->apportionment = 1;
    }
};

template<typename T>
struct MaxHeapNode {
    T element;
    long double weight;

    MaxHeapNode(T element, long double weight) { // TODO this is not Javay?
        this->element = element;
        this->weight = weight;
    }
};

template<typename T>
class MaxHeap {
   vector<unique_ptr<MaxHeapNode<T>>> nodes;
   long double (*weightFunction)(T);
   long numElements = 0;  // TODO: remove

    long parentIndex(long childIndex) {
        if (childIndex == 0) {
            return -1;
        }
        return (childIndex - 1) / 2;
    }

    long leftChildIndex(long parentIndex) {
        long childIndex = 2 * parentIndex + 1;
        if (childIndex >= this->numElements) {
            return -1;
        }
        return childIndex;
    }

    long rightChildIndex(long parentIndex) {
        long childIndex = 2 * parentIndex + 2;
        if (childIndex >= this->numElements) {
            return -1;
        }
        return childIndex;
    }

    long double getWeight(long index) {
        return (*this->nodes[index]).weight;
    }

    void swap(long index1, long index2) {
        (this->nodes[index1]).swap(this->nodes[index2]);
    }

   public:
   MaxHeap(long double (*weightFunction)(T), vector<T>& initialElements) {
       this->weightFunction = weightFunction;
       for (auto&& element : initialElements) {
           this->add(element);
       }
   }

   shared_ptr<State> pop() {
       shared_ptr<State> result = (*(this->nodes[0])).element;
       if (this->numElements > 1) {
           (this->nodes[0]).swap(this->nodes[this->numElements -1]);
       }
       this->nodes.pop_back();
       this->numElements -= 1;

       long parentIndex = 0;
        long leftChildIndex = this->leftChildIndex(parentIndex);
        long rightChildIndex = this->rightChildIndex(parentIndex);
        while((leftChildIndex >= 0 || rightChildIndex >= 0) ) {
            long childIndex = 0;
            if (leftChildIndex < 0) {
                childIndex = rightChildIndex;
            } else if (rightChildIndex < 0) {
                childIndex = leftChildIndex;
            } else if (getWeight(leftChildIndex) < getWeight(rightChildIndex)) {
                childIndex = rightChildIndex;
            } else {
                childIndex = leftChildIndex;
            }
            if (getWeight(childIndex) <= getWeight(parentIndex)) {
                break;
            }
            this->swap(childIndex, parentIndex);
            parentIndex = childIndex;
            leftChildIndex = this->leftChildIndex(parentIndex);
            rightChildIndex = this->rightChildIndex(parentIndex);
            // cout << parentIndex << " " << leftChildIndex <<  "inf loop!" << endl;
        }

        // re-adjust the heap
       return result;
   }

   void add(T element) {
       this->numElements = this->numElements + 1;
       nodes.push_back(unique_ptr<MaxHeapNode<T>>(new MaxHeapNode<T>(element, this->weightFunction(element))));
        long childIndex = this->numElements - 1;
        long parentIndex = this->parentIndex(childIndex);
        while (parentIndex >= 0 and (*this->nodes[parentIndex]).weight < (*this->nodes[childIndex]).weight ) {
            this->swap(parentIndex, childIndex);
            childIndex = parentIndex;
            parentIndex = this->parentIndex(childIndex);
        }
   }
};



long double calculatePriority(shared_ptr<State> state) {
    return state->population / sqrtl(state->apportionment * (state->apportionment + 1)); 
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

vector<shared_ptr<State>> getStatesFromCSV(istream& inputStream, bool skipFirstLine = true) {
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

int main()
{
    ifstream rfile;
    rfile.open("census2010.csv");
    vector<shared_ptr<State>> states = getStatesFromCSV(rfile);
    rfile.close();
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
