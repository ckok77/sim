#include "u_watch.h"

void u_watch::start() {
  _start = std::chrono::high_resolution_clock::now();
  is_first = false;
}

bool u_watch::is_start() {
  return !is_first;
}

uint64_t u_watch::elapse(float scal) {
  if (scal <= 0.01)
    scal = 1.;
  auto _end = std::chrono::high_resolution_clock::now();
  auto microseconds =
      std::chrono::duration_cast<std::chrono::microseconds>(_end - _start);
  return uint64_t(microseconds.count() / 1000 / scal);
}
