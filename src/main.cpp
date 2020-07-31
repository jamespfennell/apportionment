#include <fstream>
#include <iostream>
#include <unordered_map>

#include "../include/apportionment.hpp"
#include "../include/csv.hpp"
#include "../lib/cxxopts-v2.2.0.hpp"

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
// - verify historical apportionments!
// - write CSV priortiy outputter
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

void writeSeatBySeatAsCsv(ostream &outputStream,
                              const vector<State> &states,
                              const int &maxApportionment) {
  ApportionmentSession session(states, ArithmeticMethod::EXACT);      

    vector<string> headers = {"entity", "entity_seat", "house_seat", "priority_value"};
  CsvWriter csvWriter = CsvWriter(outputStream, headers);

    for (int i = states.size() + 1; i <= maxApportionment; i++) {
      ApportionedSeat apportionedSeat = session.apportionSeat();
    csvWriter.setCell("entity", apportionedSeat.state.name);
    csvWriter.setCell("entity_seat", to_string(apportionedSeat.stateSeat));
    csvWriter.setCell("house_seat", to_string(apportionedSeat.houseSeat));
    csvWriter.setCell("priority_value", to_string(apportionedSeat.priorityNumber));
    csvWriter.endRow();
    }
 }




class Options {

  ifstream inputFile;
  std::unique_ptr<CsvReader> csvReader;

public:

  bool seatBySeat;
  std::string stateName;
  std::string popName;
  long minSeats;
  long maxSeats;

  CsvReader& getCsvReader() {
    return *csvReader;
  }

  istream& inputStream() {
    if (inputFile.is_open()) {
      return inputFile;
    }
    return std::cin;
  }

  Options(int argc, char *argv[]) {
    cxxopts::Options options("apportionment",
                             "Hungington-Hill Apportionment Calculator");
    options.add_options()("m,min", "Minimum number of seats to apportion",
                          cxxopts::value<long>()->default_value("435"))(
        "M,max", "Maximum number of seats of apportion",
        cxxopts::value<long>()->default_value("435"))(
        "i,input", "Input CSV file name (if omited, input is read from stdin)",
        cxxopts::value<std::string>())(
        "s,state-column",
        "Name of CSV column containing state "
        "names (if omited, first column is used)",
        cxxopts::value<std::string>())(
        "p,pop-column",
        "Name of CSV column containing populations (if omited, second column "
        "is "
        "used)",
        cxxopts::value<std::string>())(
        "o,output", "Output file name (if omited, output is written to stdout)",
        cxxopts::value<std::string>())(
        "seat-by-seat", "Instead of outputting apportinment, output a report",
        cxxopts::value<bool>())("help", "Print help");
    auto result = options.parse(argc, argv);
    if (result.count("help")) {
      std::cout << options.help({"", "Group"}) << std::endl;
      exit(0);
    }

    if (result.count("input")) {
      std::string inputFileName = result["input"].as<std::string>();
      inputFile.open(inputFileName);
      if(!inputFile.is_open()) {
        std::cerr << "File '" << inputFileName << "' does not exist" << std::endl;
        exit(1);
      }
    }

    minSeats = result["min"].as<long>();
    maxSeats = result["max"].as<long>();

    if (inputFile.is_open()) {
      csvReader = std::make_unique<CsvReader>(CsvReader(inputFile));
    } else {
      csvReader = std::make_unique<CsvReader>(CsvReader(std::cin));
    }

    if (result.count("state-column")) {
      stateName = result["state-column"].as<std::string>();
    } else {
      stateName = (*csvReader).getHeader(0);
    }

    if (result.count("pop-column")) {
      popName = result["pop-column"].as<std::string>();
    } else {
      popName = (*csvReader).getHeader(1);
    }

    if (result.count("seat-by-seat")) {
      seatBySeat = true;
    } else {
      seatBySeat = false;
    }

  }

  ~Options() {
    if (inputFile.is_open()) {
      inputFile.close();
    }
  }
};

int main(int argc, char *argv[]) {
  Options options(argc, argv);
  vector<State> states =
      readStatesFromCsv(options.getCsvReader(), options.stateName, options.popName);

  //writeApportionmentsAsCsv(cout, states, result["min"].as<int>(),
  //                         result["max"].as<int>());
  //writeApportionmentsAsCsv(cout, states, options.minSeats, options.maxSeats);
  writeSeatBySeatAsCsv(cout, states, options.maxSeats);
  return 0;
}
