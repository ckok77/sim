#pragma once
/*
 * 由于裸指针不含有大小信息，所以自定义指针
 * 用于连续存储的 代替指针
 */
#pragma once
#include <cassert>
#include <fstream>
#include "u_base.h"

template <typename T>
void set_nil(T* p, i64 l = 1) {
  if (p == nullptr || l == 0)
    return;
  if (l > 1)
    delete[] p;
  else
    delete p;
  p = nullptr;
}
template <typename T>
T* new_fil(i64 l = 1) {
  assert(l != 0);
  T* p = new T[l];
  memset(p, 0, sizeof(T) * l);
  return p;
}

void new_bits(bit8* p, i32 old_len, i32 new_len);

void set_nil(bit8* p, i32& l);

void new_fil(bit8* p, i32 l);

void copy_chars(tract_string& des, const char* src, i32 _s);

class usr_continuation_reader {
 private:
  bit8 *_start = nullptr, *_end = nullptr;
  i64 _size = 0;

 public:
  usr_continuation_reader(bit8* n, i64 l = 0) {
    _end = _start = n;
    _size = l;
  }

  bit8* read(i64 step) {
    if (step < 0)
      assert(_end + step >= _start);
    if (step > 0)
      assert(_end - _start + step <= _size);

    bit8* p = _end;
    _end += step;
    return p;
  }

  bit8* set_pos(i64 pos) {
    assert(pos >= 0 && pos < _size);

    _end = _start + pos;
    return _end;
  }

  template <typename T>
  void to_(T* des, i64 len = 1) {
    bit8* p = read(sizeof(T) * len);
    if (len == 1)
      *des = *(T*)p;
    else
      memcpy((bit8*)des, p, sizeof(T) * len);
  }

  template <typename T>
  void from_(T* src, i64 len = 1) {
    bit8* p = read(sizeof(T) * len);
    if (len == 1)
      *(T*)p = *src;
    else
      memcpy(p, (bit8*)src, sizeof(T) * len);
  }
};

template <typename T>
struct usr_ptr {
  i64 _size = 0;
  T* _data = nullptr;
  //
  explicit usr_ptr(i64 size) { _new(size); }
  usr_ptr() {
    _size = 0;
    _data = nullptr;
  }
  ~usr_ptr() { _release(); }

  void _nil_pointer() { set_nil<T>(_data, _size); }
  void _release() {
    _nil_pointer();
    _size = 0;
  }

  void _new(i64 new_size) {
    if (new_size <= 0) {
      _release();
      return;
    }
    if (new_size == _size)
      return;
    T* p = new T[new_size];
    memset((bit8*)p, 0, new_size * sizeof(T));
    if (_size > 0 && _data != nullptr) {
      memcpy((bit8*)p, _data,
             (new_size > _size ? _size : new_size) * sizeof(T));
      _nil_pointer();
    }
    _size = new_size;
    _data = p;
  }

  T& operator[](i64 index) {
    assert(_size > 0 && index >= 0 && index < _size);
    return _data[index];
  }
  void _push(const T t) {
    _new(_size + 1);
    memcpy((bit8*)&_data[_size - 1], (bit8*)&t, sizeof(T));
  }
  void _push(T* _p, i64 _s) {
    if (_p == nullptr || _s <= 0)
      return;
    i64 i = _size;
    _new(_size + _s);
    memcpy((bit8*)&_data[i], (bit8*)_p, _s * sizeof(T));
  }
  i64 _length() { return _size; }
};

using bit_ptr = usr_ptr<bit8>;

int usr_write(const string& file_name, const bit_ptr& out_data);

i64 file_size(const string& file_name);

int usr_read(const string& file_name, bit_ptr& indata);

//-----------------------------------------------------------------

template <typename T>
bit_ptr& operator<<(bit_ptr& f, usr_ptr<T>& obj) {
  bit8* p0 = new bit8[obj._size * sizeof(T) + sizeof(i64) + f._size];
  bit8* p1 = p0;
  if (f._data != nullptr && f._size > 0) {
    memcpy(p0, f._data, f._size);
    p1 += f._size;
  }
  memcpy(p1, &obj._size, sizeof(i64));
  p1 += sizeof(i64);

  if (obj._size > 0) {
    memcpy(p1, obj._data, sizeof(T) * obj._size);
  }
  f._nil_pointer();
  f._size += obj._size * sizeof(T) + sizeof(i64);
  f._data = p0;
  return f;
}

template <typename T>
bit_ptr& operator>>(bit_ptr& f, usr_ptr<T>& obj) {
  if (f._size < (i64)sizeof(i64) || f._data == nullptr)
    return f;
  bit8* p0 = nullptr;
  bit8* p1 = f._data;
  obj._size = *(i64*)p1;
  p1 += sizeof(i64);
  if (obj._size > 0) {
    obj._data = new T[obj._size];
    memcpy((bit8*)&obj._data[0], p1, obj._size * sizeof(T));
  }
  f._size = f._size - obj._size * sizeof(T) - sizeof(i64);
  if (f._size > 0) {
    p0 = new bit8[f._size];
    p1 += sizeof(T) * obj._size;
    memcpy(p0, p1, f._size);
  } else
    f._size = 0;
  f._nil_pointer();
  f._data = p0;
  return f;
}

template <typename T>
bit_ptr& operator<<(bit_ptr& f, const T& obj) {
  i64 unit_size = sizeof(T);
  i64 obj_len = 1;
  bit8* p0 = new bit8[unit_size * obj_len + sizeof(i64) + f._size];
  bit8* p1 = p0;
  if (f._data != nullptr && f._size > 0) {
    memcpy(p0, f._data, f._size);
    p1 += f._size;
  }
  memcpy(p1, &obj_len, sizeof(i64));
  p1 += sizeof(i64);

  memcpy(p1, (bit8*)&obj, unit_size * obj_len);

  f._nil_pointer();
  f._size += unit_size * obj_len + sizeof(i64);
  f._data = p0;
  return f;
}

template <typename T>
bit_ptr& operator>>(bit_ptr& f, T& obj) {
  if (f._size < (i64)sizeof(i64) || f._data == nullptr)
    return f;
  i64 unit_size = sizeof(T);
  i64 obj_len;
  bit8* p0 = nullptr;
  bit8* p1 = f._data;

  obj_len = *(i64*)p1;
  assert(1 == obj_len);
  p1 += sizeof(i64);
  memcpy((bit8*)&obj, p1, unit_size * obj_len);

  f._size = f._size - unit_size * obj_len - sizeof(i64);
  if (f._size > 0) {
    p0 = new bit8[f._size];
    p1 += unit_size * obj_len;
    memcpy(p0, p1, f._size);
  } else
    f._size = 0;
  f._nil_pointer();
  f._data = p0;
  return f;
}
