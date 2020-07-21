#include <iostream>
#include <unordered_map>
#include <fstream>

#include "../include/csv.hpp"
#include "../include/apportionment.hpp"

long double identity(int i) {
    return i;
}

using namespace std;

vector<State> readStatesFromCsv(CsvReader& csvReader, const string& nameHeader, const string& populationHeader) {
    vector<State> states;
    while (csvReader) {
        unordered_map<string, string> row = csvReader.getRow();
        long long population = stoll(row[populationHeader]);  // TODO: what if there's an error here?
        State state = State{row[nameHeader], population};
        states.push_back(state);
    }
    return states;
}
// NEXT STEPS
// - use this hash function everywhere
// - expose a getCurrentApportionment in the ApportionmentSession return the map by const reference
// - expost an apportionMultipleSeats(int numSeats);
// - accept command line arguments

void writeApportionmentsToCsv(ostream& outputStream, const vector<State>& states,
const int& minApportionment, const int& maxApportionment) {


    ApportionmentSession session = ApportionmentSession(states);
    unordered_map<State, unordered_map<int, int>> stateToNumSeatsToApportionment;
    for (int i=states.size() + 1; i<= maxApportionment; i++) {
        ApportionedSeat apportionedSeat = session.apportionSeat();
        if (i < minApportionment) {
            continue;
        }
        const unordered_map<State, long>& stateToSeats = session.getCurrentApportionment();
        for(const auto& state: states) {
            stateToNumSeatsToApportionment[state][i] = stateToSeats.at(state);
        }
    }


    vector<string> headers = {"entity"};
    for (int i = minApportionment; i <= maxApportionment; i++) {
        headers.push_back("apportionment_" + to_string(i));
    }
    CsvWriter csvWriter = CsvWriter(outputStream, headers);

    for (const auto& state : states) {
        csvWriter.setCell("entity", state.name);
        for (int i= minApportionment; i <= maxApportionment; i ++ ) {
            csvWriter.setCell("apportionment_" + to_string(i), 
                to_string(stateToNumSeatsToApportionment[state][i])
            );
        }
        csvWriter.endRow();
    }

}

int main()
{
    ifstream rfile1;
    rfile1.open("data/2010_populations.csv");
    CsvReader csvReader = CsvReader(rfile1);
    string nameHeader = csvReader.getHeader(0);
    string populationHeader = csvReader.getHeader(1);
    vector<State> states = readStatesFromCsv(csvReader, nameHeader, populationHeader);
    rfile1.close();

    writeApportionmentsToCsv(cout, states, 435, 435);
    return 0;

    unordered_map<string, int> stateNameToNumberOfSeats; 
    for (const auto& state: states) {
        stateNameToNumberOfSeats.insert(make_pair(state.name, 1));
    }
    ApportionmentSession session = ApportionmentSession(states);
    for (int i=51; i<=435; i++) {
        ApportionedSeat apportionedSeat = session.apportionSeat();
        // cout << "Apportioned " << apportionedSeat.state.name << endl;
        stateNameToNumberOfSeats[apportionedSeat.state.name] = apportionedSeat.stateSeat;
    }

    CsvWriter csvWriter = CsvWriter(cout, {"entity", "apportionment_435"});
    for (const auto& state: states) {
        csvWriter.setCell("entity", state.name);
        csvWriter.setCell("apportionment_435", to_string(stateNameToNumberOfSeats.at(state.name)));
        csvWriter.endRow();
    }
}
