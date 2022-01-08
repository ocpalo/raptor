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
        lat1 = (lat1 * (M_PI / 180));
        lon1 = (lon1 * (M_PI / 180));
        lat2 = (lat2 * (M_PI / 180));
        lon2 = (lon2 * (M_PI / 180));
        float x = std::cos(lat2) * std::sin(lon2 - lon1);
        float y = std::cos(lat1) * std::sin(lat2) - std::sin(lat1) * std::cos(lat2) * std::cos(lon2 - lon1);
        float bearing = std::atan2(x,y);
        return std::fmod(bearing*180/pi + 360, 360);
    }
}