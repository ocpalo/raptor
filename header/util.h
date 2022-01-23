#include <string>
#include <vector>

namespace util {
double haversine(double x1, double y1, double x2, double y2);
double bearing(double lat1, double lon1, double lat2, double lon2);
std::vector<std::string> split(std::string const& line, char delimiter);
}  // namespace util