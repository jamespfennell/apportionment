#include <iostream>
#include <unordered_map>
#include <fstream>

#include "csv.cpp"
#include "../include/apportionment.hpp"

long double identity(int i) {
    return i;
}


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
