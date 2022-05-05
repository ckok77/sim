#pragma once

#include <chrono>

class u_watch {
 private:
  bool is_first{true};
  std::chrono::time_point<std::chrono::high_resolution_clock> _start;

 public:
  void start();
  //大于1时计时变慢 可让外部调速
  uint64_t elapse(float scal = 1.);
  bool is_start();
};
