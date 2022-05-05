//计算表的模拟量计算

#pragma once
#include "../s_table.h"
#include "u_const.h"

struct s_f_record {
  int id_state{0};
  bool b_state{false};
  int id_value{0};
  float f_value{0.f};
};

namespace tick_fspace {

void record_bit(int d, bool v, s_f_record& s);

//插值计算
float LookupEvaluation(int _r, s_tab& _t, s_pack& _p);
//模拟量计算
float DirectnessGetValue(int _r, s_tab& _t, s_pack& _p);
//对于非感性负载,电压和电流是瞬间变化的.
float SubtrationCount(int _r, s_tab& _t, s_pack& _p);
//模拟量信号关断处理
float TakeOffCourse(int _r, s_tab& _t, s_pack& _p);

// public
//参数:_r行号 _t数据库数据 _p数据包 _s上一次的数据 status本行的数字量计算结果
float tick_f(int _r, s_tab& _t, s_pack& _p, s_f_record& _s, bool status);

};  // namespace tick_fspace
