#ifndef TIMER_H
#define TIMER_H

#include <chrono>

namespace util {
class Timer {
 public:
  void start();
  void stop();
  std::chrono::milliseconds elapsedMilliseconds();

 private:
  std::chrono::time_point<std::chrono::system_clock> _startTime;
  std::chrono::time_point<std::chrono::system_clock> _endTime;
  bool _running = false;
};
}  // namespace util

#endif