#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

class CsvReader {

  std::unordered_map<int, std::string> indexToHeader;
  std::istream *inputStream;
  bool hasNextLine = false;
  std::string nextLine;

  static std::vector<std::string> splitString(const std::string &input);

  void readNextLine();

public:
  CsvReader(std::istream &inputStream);

  std::string getHeader(const int &index) const;

  bool hasHeader(const std::string &header) const;

  operator bool() const;

  std::unordered_map<std::string, std::string> getRow();
};

class CsvWriter {
  std::ostream *outputStream;
  std::vector<std::string> headers;
  std::unordered_map<std::string, std::string> headerToCellValue;

public:
  CsvWriter(std::ostream &outputStream,
            const std::vector<std::string> &headers);

  void setCell(const std::string &header, const std::string &value);

  void endRow();
};
