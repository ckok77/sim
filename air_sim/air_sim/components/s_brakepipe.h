#pragma once
/*
 * 制动管
 */

#include "public_h.h"
#include "s_brakevalve.h"
#include "s_gasjar.h"

//常用制动管路 /常用制动管路    G阀=转向架一;S阀=转向架二
struct s_brakepipe {
  s_brakevalve valve[2];  // G阀 S阀
  s_gasjar _gasjar;       //储气缸类

  //滑行制动
  switchs switch_G011a{switchs::on};  //第一架a轴防滑阀状态	0:失电 1:得电
  switchs switch_G011b{switchs::on};  //第一架b轴防滑阀状态	0:失电 1:得电

  s_brakepipe();
};
