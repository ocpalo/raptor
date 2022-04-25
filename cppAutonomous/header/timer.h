#ifndef TIMER_H
#define TIMER_H

#include <chrono>

namespace drone::util {
class Timer {
 public:
  void start();
  void stop();
  std::chrono::milliseconds elapsedMilliseconds();
  bool running();

 private:
  std::chrono::time_point<std::chrono::system_clock> _startTime;
  std::chrono::time_point<std::chrono::system_clock> _endTime;
  bool _running = false;
};
}  // namespace drone::util

#endif