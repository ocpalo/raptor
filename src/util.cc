#include <cmath>
#include "util.h"

namespace util {
    # define M_PI 3.14159265358979323846

    double haversine(double lat1, double lon1, double lat2, double lon2) {
        // distance between latitudes
        // and longitudes
        double dLat = (lat2 - lat1) *
                      M_PI / 180.0;
        double dLon = (lon2 - lon1) *
                      M_PI / 180.0;
 
        // convert to radians
        lat1 = (lat1) * M_PI / 180.0;
        lat2 = (lat2) * M_PI / 180.0;
 
        // apply formulae
        double a = std::pow(std::sin(dLat / 2), 2) +
                   std::pow(std::sin(dLon / 2), 2) *
                   std::cos(lat1) * std::cos(lat2);
        double rad = 6371;
        double c = 2 * std::asin(std::sqrt(a));
        return rad * c;
    }
}