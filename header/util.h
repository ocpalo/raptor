#ifndef UTIL_H
#define UTIL_H

#include <cmath>
#include <numbers>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

namespace drone::util {

template <typename Head, typename... Tail>
std::string get_string(char delimeter, const Head &head, const Tail &... tail) {
  std::stringstream stream;
  stream << head;
  ((stream << delimeter << tail), ...);
  return stream.str();
}

template <std::floating_point T>
T haversine(T src_lat, T src_lon, T dst_lat, T dst_lon) {
  T lat = (dst_lat - src_lat) * std::numbers::pi_v<T> / 180.0;
  T lon = (dst_lon - src_lon) * std::numbers::pi_v<T> / 180.0;

  T rad_src_lat = src_lat * std::numbers::pi_v<T> / 180.0;
  T rad_dst_lat = dst_lat * std::numbers::pi_v<T> / 180.0;

  T a = std::pow(std::sin(lat / 2), 2) + std::pow(std::sin(lon / 2), 2) *
                                             std::cos(rad_src_lat) *
                                             std::cos(rad_dst_lat);
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
template <std::floating_point T>
T bearing(T lat1, T lon1, T lat2, T lon2) {
  T lat = (lat1 * (std::numbers::pi_v<T> / 180));
  T lon = (lon1 * (std::numbers::pi_v<T> / 180));
  T dlat = (lat2 * (std::numbers::pi_v<T> / 180));
  T dlon = (lon2 * (std::numbers::pi_v<T> / 180));
  T x = std::cos(dlat) * std::sin(dlon - lon);
  T y = std::cos(lat) * std::sin(dlat) -
        std::sin(lat) * std::cos(dlat) * std::cos(dlon - lon);
  T bearing = std::atan2(x, y);
  return std::fmod(bearing * 180 / std::numbers::pi_v<T> + 360, 360);
}
std::vector<std::string> split(std::string const &line, char delimiter);
}  // namespace drone::util

#endif
