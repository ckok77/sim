#include "u_tick_c.h"

namespace tick_cspace {
void record_bit(int d, bool v, s_b_record& s) {
  s.id_state = d;
  s.b_state = v;
}

//得到计算表格(1)某一行的输入逻辑状态(根据3~6列) 与
bool get_input_colass(int r, s_tab& _t, s_pack& _p) {
  if (!in_range(r, 0, int(_t.at.tbs.size())))
    return false;
  for (auto i = 3; i < 7; i++) {
    int u = int(_t.at.tbs[r].value[i]);
    if (u != 0) {
      bool b = g_bits(u, _p.i_c);
      if ((b && u < 0) || (!b && u > 0))
        return false;
    }
  }
  return true;
}
//得到计算表格(1)某一行的内部逻辑状态(根据7~9列) 与
bool get_inside_colass(int r, s_tab& _t, s_pack& _p) {
  if (!in_range(r, 0, int(_t.at.tbs.size())))
    return false;
  for (auto i = 7; i < 10; i++) {
    int u = int(_t.at.tbs[r].value[i]);
    if (u != 0) {
      bool b = g_bits(u, _p._c._data);
      if ((b && u < 0) || (!b && u > 0))
        return false;
    }
  }
  return true;
}
bool tick_c(int _r, s_tab& _t, s_pack& _p, s_b_record& _s) {
  bool b_state = get_inside_colass(_r, _t, _p) && get_input_colass(_r, _t, _p);
  int old_row = _r <= 0 ? 0 : _r - 1;
  int id = int(_t.at.tbs[_r].value[管路逻辑号]);
  if (id == 0) {  //啥都不是 把以前的事做了
    if (old_row > 0)
      s_bits(_p._c._data, _s.id_state, _s.b_state);

  } else
    record_bit(id, b_state, _s);  //记录下来
  return b_state;
}

}  // namespace tick_cspace