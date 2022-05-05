//脚踏泵
#pragma once
#include "public_h.h"

struct s_footpump {
  works work_this;
  switchs switch_A0910;  //脚踏泵开关隔离塞门(状态: 1:关闭 0:打开)
  float dSupplySpeed{10.};  //脚踏泵单次大风量    单位:L
  float dTotalCubage{0.};   //打风总量

  s_footpump();
  void init(param_tab e);
  void update(void);  //更新状打风量
};
