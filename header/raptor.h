#include <string>

#include "base_drone.h"

namespace drone {

class raptor : public base_drone {
 public:
  raptor(std::string const& connection_url);
  ~raptor() = default;

  bool move2(float heading);
};
}  // namespace drone