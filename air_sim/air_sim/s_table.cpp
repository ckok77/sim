#include "s_table.h"

//奇葩的计算方式
int small_tab::get_count_sign() {
  if (flag[0] == 0 && flag[1] == 0)
    return 0;
  if ((flag[0] != 0) && (flag[1] == 0) && (flag[2] == 0) && (flag[3] == 0))
    return 1;
  if ((flag[0] == 0) && (flag[1] != 0) && (flag[2] == 0) && (flag[3] == 0))
    return 2;
  if ((flag[0] != 0) && (flag[1] != 0) && (flag[2] == 0) && (flag[3] == 0))
    return 3;
  if ((flag[0] != 0) && (flag[1] != 0) && (flag[2] != 0) && (flag[3] == 0))
    return 4;
  if ((flag[0] != 0) && (flag[1] != 0) && (flag[2] == 0) && (flag[3] != 0))
    return 5;
  if ((flag[0] != 0) && (flag[1] == 0) && (flag[2] != 0) && (flag[3] == 0))
    return 6;
  if ((flag[0] == 0) && (flag[1] != 0) && (flag[2] == 0) && (flag[3] != 0))
    return 7;
  return 8;
}

// id_x第几行 id_y第几列
float small_tab::get_v(int id_x, int id_y) {
  return av[id_x * ay._size + id_y];
}

// flag = 1 已知x float 固定y值（默认=第2列）的求值
float small_tab::get_v(float _x, int id_y) {
  for (auto i = 0; i < ax._size; i++) {
    if (is_equal(ax[i], _x))
      return get_v(i, id_y);
    if (ax[i] > _x) {
      auto x0 = ax[i - 1];
      auto y0 = get_v(i - 1, id_y);
      auto x1 = ax[i];
      auto y1 = get_v(i, id_y);
      return interpolate(x0, y0, x1, y1, _x);
    }
  }
  return 0.f;
}

// flag = 1 已知x float 和y float 的求值
float small_tab::get_v(float _x, float _y) {
  usr_ptr<float> f0{ay._size};
  //先插一次
  for (auto i = 0; i < ay._size; i++) {
    f0[i] = get_v(_x, i);
  }
  if (f0._size == 1)
    return f0[0];
  for (auto i = 0; i < f0._size; i++) {
    if (is_equal(ay[i], _y))
      return f0[i];
    if (ay[i] > _y) {
      auto x0 = ay[i - 1];
      auto y0 = f0[i - 1];
      auto x1 = ay[i];
      auto y1 = f0[i];
      return interpolate(x0, y0, x1, y1, _y);
    }
  }
  return 0.f;
}

//根据x的值得到变化率
float small_tab::get_r(float _x) {
  for (auto i = 0; i < ax._size; i++) {
    if (is_equal(ax[i], _x))
      return ar[i];
    if (ax[i] > _x) {
      auto x0 = ax[i - 1];
      auto y0 = ar[i - 1];
      auto x1 = ax[i];
      auto y1 = ar[i];
      return interpolate(x0, y0, x1, y1, _x);
    }
  }
  return 0.f;
}

int tick_tabs::max_col(int c) {
  int imax = 0;
  if (!in_range<int>(c, 0, tick_col_count))
    return 0;
  for (auto i = 0; i < tbs.size(); i++) {
    auto n = int(tbs[i].value[c]);
    if (imax < n)
      imax = n;
  }
  return imax;
}
