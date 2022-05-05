//计算表的数字量计算

#pragma once
#include "public_h.h"

//为了能连续计算相同的值 所以把上一次的值存储在本结构体中
struct s_b_record {
  int id_state{0};
  bool b_state{false};
};

namespace tick_cspace {

//记录上次的值
void record_bit(int d, bool v, s_b_record& s);
//得到计算表格(1)某一行的输入逻辑状态(根据3~6列) 与
bool get_input_colass(int r, s_tab& _t, s_pack& _p);
//得到计算表格(1)某一行的内部逻辑状态(根据7~9列) 与
bool get_inside_colass(int r, s_tab& _t, s_pack& _p);
// public
//参数:_r行号 _t数据库数据 _in内部的存储数据 _s上一次的数据
bool tick_c(int _r, s_tab& _t, s_pack& _p, s_b_record& _s);

};  // namespace tick_cspace
