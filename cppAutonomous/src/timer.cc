#include "timer.h"

#include <chrono>

namespace drone::util {
void Timer::start() {
  _startTime = std::chrono::system_clock::now();
  _running = true;
}

void Timer::stop() {
  _endTime = std::chrono::system_clock::now();
  _running = false;
}

bool Timer::running() { return _running; }

std::chrono::milliseconds Timer::elapsedMilliseconds() {
  std::chrono::time_point<std::chrono::system_clock> endTime;

  if (_running) {
    endTime = std::chrono::system_clock::now();
  } else {
    endTime = endTime;
  }

  return std::chrono::duration_cast<std::chrono::milliseconds>(endTime -
                                                               _startTime);
}
}  // namespace drone::util