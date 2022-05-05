#include "u_base.h"

void nstring::from_string(std::string s) {
  if (nullptr != c)
    delete[] c;
  c = new char[s.length() + 1];
  strcpy_s(c, s.length() + 1, s.c_str());
}

void nstring::from_strings(std::vector<std::string> ss) {
  if (ss.size() <= 0)
    return;
  from_string(Join(ss, "|"));
}

std::string nstring::to_string() {
  return std::string(c);
}

nstring::nstring() {
  size = 0;
  c = nullptr;
}

nstring::~nstring() {
  if (c != nullptr)
    delete[] c;
}
