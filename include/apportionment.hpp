
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

vector<shared_ptr<State>> getStatesFromCSV(istream& inputStream, bool skipFirstLine);

long double calculatePriority(shared_ptr<State> state);
