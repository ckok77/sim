#include "u_ptr.h"

void new_bits(bit8* p, i32 old_len, i32 new_len) {
  set_nil(p, old_len);
  p = new bit8[new_len];
  memset(p, 0, new_len);
}
void set_nil(bit8* p, i32& l) {
  if (p == nullptr)
    return;
  if (l > 1)
    delete[] p;
  else
    delete p;
  p = nullptr;
  l = 0;
}
void new_fil(bit8* p, i32 l) {
  set_nil(p, l);
  p = new bit8[l];
  memset(p, 0, l);
}
void copy_chars(tract_string& des, const char* src, i32 _s) {
  if (src == nullptr || _s == 0)
    return;
  memset(des, 0, tract_str_len);
  i32 i{0};
  for (i = 0; src[i] != '\0' && i < _s - 1 && i < tract_str_len - 1; i++)
    des[i] = src[i];
  des[i] = '\0';
}

int usr_write(const string& file_name, const bit_ptr& out_data) {
  ofstream ostrm(file_name, ios::out | ios::binary | ios::trunc);
  char* p = reinterpret_cast<char*>(out_data._data);  // important!
  ostrm.write(p, (signed long long)out_data._size);
  return 0;
}

i64 file_size(const string& file_name) {
  setlocale(LC_ALL, "Chinese-simplified");
  std::ifstream in(file_name.c_str());
  if (!in.is_open())
    return 0;
  in.seekg(0, std::ios_base::end);
  std::streampos sp = in.tellg();
  setlocale(LC_ALL, "C");
  return sp;
}

int usr_read(const string& file_name, bit_ptr& indata) {
  indata._release();
  i64 i_len = file_size(file_name);
  assert(i_len > 0);
  indata._new(i_len);
  std::ifstream istrm(file_name, std::ios::binary);
  istrm.read(reinterpret_cast<char*>(indata._data),
             (unsigned long long)indata._size);
  return 0;
}