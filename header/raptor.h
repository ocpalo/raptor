#include "base_drone.h"
#include <string>

namespace drone {

    class raptor : public base_drone 
    {
    public:
        raptor(std::string const& connection_url);
        ~raptor() = default;

        bool move(float heading);
    };
}