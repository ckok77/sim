//储气风缸
#pragma once
#include "public_h.h"

struct s_gasjar {
  switchs switch_B05;  //储气风缸单向阀 B05状态 (状态: 0:故障  1:正常)
  float dCubage{100.};          //储气风缸容积
  float dPressure{5.f};         //储气风缸压强
  float dRatingPressure{6.1f};  //储气风缸额定压强
  float dcDepositCapacityChangeSpeed;
  float dFillInCubage{0.f};  //总风管向制动储气风缸充气 气压值

  s_gasjar();
  void init(param_tab e);

  //更新储气风缸气压
  void update(float dDeltaT,
              float dMainRandPressure,
              float dDepositCapacityToBrakePipeCubage);
  //总风管向制动储气风缸充气量  单位:L
  float GetMainRandToDepositCapacityCubage(void);
};