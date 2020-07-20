#include <iostream>
#include <unordered_map>
#include <fstream>

#include "../include/csv.hpp"
#include "../include/apportionment.hpp"

long double identity(int i) {
    return i;
}

using namespace std;

vector<State> readStates(CsvReader& csvReader, const string& nameHeader, const string& populationHeader) {
    vector<State> states = vector<State>{};
    while (csvReader) {
        unordered_map<string, string> row = csvReader.getRow();
        long long population = stoll(row[populationHeader]);  // TODO: what if there's an error here?
        State state = State{row[nameHeader], population};
        states.push_back(state);
    }
    return states;
}
// NEXT STEPS
// - write the CSV writer class and output the CSV file, travis should be green again
// - accept command line arguments
int main()
{
    ifstream rfile1;
    rfile1.open("data/2000_populations.csv");
    CsvReader csvReader = CsvReader(rfile1);
    string nameHeader = csvReader.getHeader(0);
    string populationHeader = csvReader.getHeader(1);
    vector<State> states = readStates(csvReader, nameHeader, populationHeader);
    for (auto state: states) {
        cout << state.name << " " << state.population << endl;
    }
    rfile1.close();

    ApportionmentSession session = ApportionmentSession(states);
    for (int i=51; i<=435; i++) {
        ApportionedSeat apportionedSeat = session.apportionSeat();
        cout << "Apportioned " << apportionedSeat.state.name << endl;
    }

    return 0;
}
