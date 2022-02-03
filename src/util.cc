#include "util.h"

#include <cmath>
#include <cstring>
#include <numbers>
#include <vector>

namespace util {
std::vector<std::string> split(std::string const& line, char delimiter) {
  std::vector<std::string> out;

  int start = 0;
  int end = line.find(delimiter);
  while (end != -1) {
    out.push_back(line.substr(start, end - start));
    start = end + sizeof(delimiter);
    end = line.find(delimiter, start);
  }
  out.push_back(line.substr(start, end - start));

  return out;
}
}  // namespace util