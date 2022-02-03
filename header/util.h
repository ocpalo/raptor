#ifndef UTIL_H
#define UTIL_H

#include <cmath>
#include <numbers>
#include <string>
#include <vector>

namespace util {
template <typename T>
T haversine(T lat1, T lon1, T lat2, T lon2) {
  T dLat = (lat2 - lat1) * std::numbers::pi_v<T> / 180.0;
  T dLon = (lon2 - lon1) * std::numbers::pi_v<T> / 180.0;

  lat1 = (lat1)*std::numbers::pi_v<T> / 180.0;
  lat2 = (lat2)*std::numbers::pi_v<T> / 180.0;

  T a = std::pow(std::sin(dLat / 2), 2) +
        std::pow(std::sin(dLon / 2), 2) * std::cos(lat1) * std::cos(lat2);
  T rad = 6372797.560856;
  T c = 2 * std::asin(std::sqrt(a));
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
template <typename T>
T bearing(T lat1, T lon1, T lat2, T lon2) {
  lat1 = (lat1 * (std::numbers::pi_v<T> / 180));
  lon1 = (lon1 * (std::numbers::pi_v<T> / 180));
  lat2 = (lat2 * (std::numbers::pi_v<T> / 180));
  lon2 = (lon2 * (std::numbers::pi_v<T> / 180));
  T x = std::cos(lat2) * std::sin(lon2 - lon1);
  T y = std::cos(lat1) * std::sin(lat2) -
        std::sin(lat1) * std::cos(lat2) * std::cos(lon2 - lon1);
  T bearing = std::atan2(x, y);
  return std::fmod(bearing * 180 / std::numbers::pi_v<T> + 360, 360);
}
std::vector<std::string> split(std::string const& line, char delimiter);
}  // namespace util

#endif
