#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

class CsvReader {

  std::unordered_map<int, std::string> indexToHeader;
  std::istream *inputStream;
  bool hasNextLine = false;
  std::string nextLine;

  static std::vector<std::string> splitString(std::string input);

  void readNextLine();

public:
  CsvReader(std::istream &inputStream);

  std::string getHeader(int index) const;

  bool hasHeader(std::string header) const;

  operator bool() const;

  std::unordered_map<std::string, std::string> getRow();
};