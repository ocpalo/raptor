#include "util.h"

#include <cmath>
#include <cstring>
#include <numbers>
#include <vector>

namespace util {
double haversine(double lat1, double lon1, double lat2, double lon2) {
  double dLat = (lat2 - lat1) * std::numbers::pi / 180.0;
  double dLon = (lon2 - lon1) * std::numbers::pi / 180.0;

  lat1 = (lat1)*std::numbers::pi / 180.0;
  lat2 = (lat2)*std::numbers::pi / 180.0;

  double a = std::pow(std::sin(dLat / 2), 2) +
             std::pow(std::sin(dLon / 2), 2) * std::cos(lat1) * std::cos(lat2);
  double rad = 6372797.560856;
  double c = 2 * std::asin(std::sqrt(a));
  return rad * c;
}

/*
 *   Let ‘R’ be the radius of Earth,
 *   ‘L’ be the longitude,
 *   ‘θ’ be latitude,
 *   ‘β‘ be Bearing.
 *
 *   Bearing from point A to B, can be calculated as,
 *
 *   β = atan2(X,Y),
 *
 *   where, X and Y are two quantities and can be calculated as:
 *
 *   X = cos θb * sin ∆L
 *
 *   Y = cos θa * sin θb – sin θa * cos θb * cos ∆L
 */
double bearing(double lat1, double lon1, double lat2, double lon2) {
  lat1 = (lat1 * (std::numbers::pi / 180));
  lon1 = (lon1 * (std::numbers::pi / 180));
  lat2 = (lat2 * (std::numbers::pi / 180));
  lon2 = (lon2 * (std::numbers::pi / 180));
  float x = std::cos(lat2) * std::sin(lon2 - lon1);
  float y = std::cos(lat1) * std::sin(lat2) -
            std::sin(lat1) * std::cos(lat2) * std::cos(lon2 - lon1);
  float bearing = std::atan2(x, y);
  return std::fmod(bearing * 180 / std::numbers::pi + 360, 360);
}

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