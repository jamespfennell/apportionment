#include <fstream>
#include <iostream>
#include <unordered_map>

#include "../include/apportionment.hpp"
#include "../include/csv.hpp"

long double identity(int i) { return i; }

using namespace std;

vector<State> readStatesFromCsv(CsvReader &csvReader, const string &nameHeader,
                                const string &populationHeader) {
  vector<State> states;
  while (csvReader) {
    unordered_map<string, string> row = csvReader.getRow();
    long long population =
        stoll(row[populationHeader]); // TODO: what if there's an error here?
    State state = State{row[nameHeader], population};
    states.push_back(state);
  }
  return states;
}
// NEXT STEPS
// - write CSV priortiy outputter
// - accept command line arguments
// - unsigned long long wherever possible. Document limitations!

void writeApportionmentsAsCsv(ostream &outputStream,
                              const vector<State> &states,
                              const int &minApportionment,
                              const int &maxApportionment) {
  unordered_map<State, unordered_map<int, int>> stateToNumSeatsToApportionment =
      buildApportionments(states, minApportionment, maxApportionment,
                          ArithmeticMethod::EXACT);

  vector<string> headers = {"entity"};
  for (int i = minApportionment; i <= maxApportionment; i++) {
    headers.push_back("apportionment_" + to_string(i));
  }
  CsvWriter csvWriter = CsvWriter(outputStream, headers);

  for (const auto &state : states) {
    csvWriter.setCell("entity", state.name);
    for (int i = minApportionment; i <= maxApportionment; i++) {
      csvWriter.setCell("apportionment_" + to_string(i),
                        to_string(stateToNumSeatsToApportionment[state][i]));
    }
    csvWriter.endRow();
  }
}

int main() {
  ifstream rfile1;
  rfile1.open("data/2010_populations.csv");
  CsvReader csvReader = CsvReader(rfile1);
  string nameHeader = csvReader.getHeader(0);
  string populationHeader = csvReader.getHeader(1);
  vector<State> states =
      readStatesFromCsv(csvReader, nameHeader, populationHeader);
  rfile1.close();

  writeApportionmentsAsCsv(cout, states, 435, 435);
  // writeApportionmentsAsCsv(cout, states, 51, 51);
  return 0;
}
