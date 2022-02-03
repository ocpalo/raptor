#include "timer.h"

#include <chrono>

namespace util {
void Timer::start() {
  _startTime = std::chrono::system_clock::now();
  _running = true;
}

void Timer::stop() {
  _endTime = std::chrono::system_clock::now();
  _running = false;
}

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
}  // namespace util