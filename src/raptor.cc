#include "raptor.h"
#include "base_drone.h"

#include <thread>
#include <string>
#include <chrono>

namespace drone {

    raptor::raptor(std::string const& conn_url) : base_drone(conn_url) {
    }

    bool raptor::move2(float heading) {
        if(std::abs(heading - this->_heading) > 3) {
            move_forward(3);
            set_heading(heading);
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(1s);
            return false;
        }
        return true;
    }

    
}