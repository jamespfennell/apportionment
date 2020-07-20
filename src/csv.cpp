#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "../include/csv.hpp"

using namespace std;

vector<string> CsvReader::splitString(string input) {
  vector<string> subStrings = vector<string>{};
  int leftIndex = 0;
  int rightIndex = input.find(",", leftIndex);
  while (rightIndex != string::npos) {
    subStrings.push_back(input.substr(leftIndex, rightIndex - leftIndex));
    leftIndex = rightIndex + 1;
    rightIndex = input.find(",", leftIndex);
  }
  subStrings.push_back(input.substr(leftIndex));
  return subStrings;
}

void CsvReader::readNextLine() {
  this->hasNextLine = bool(getline(*this->inputStream, this->nextLine));
}

CsvReader::CsvReader(istream &inputStream) {
  this->inputStream = &inputStream;
  string header;
  // TODO: handle CRLF file types
  if (!getline(inputStream, header)) {
    return;
  }
  vector<string> headerCells = this->splitString(header);
  int i = 0;
  for (string headerCell : headerCells) {
    this->indexToHeader[i] = headerCell;
    i++;
  }
  this->readNextLine();
}

string CsvReader::getHeader(int index) const {
  return this->indexToHeader.at(index);
}

bool CsvReader::hasHeader(string header) const {
  for (auto it : this->indexToHeader) {
    if (it.second == header) {
      return true;
    }
  }
  return false;
}

CsvReader::operator bool() const { return this->hasNextLine; }
unordered_map<string, string> CsvReader::getRow() {
  // TODO: what if it doesn't have a next row?
  vector<string> rowCells = this->splitString(this->nextLine);
  unordered_map<string, string> headerToRowCell =
      unordered_map<string, string>{};
  for (auto it : this->indexToHeader) {
    headerToRowCell[it.second] = rowCells[it.first];
  }
  this->readNextLine();
  return headerToRowCell;
}