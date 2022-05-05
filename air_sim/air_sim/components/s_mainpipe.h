#pragma once
#include "public_h.h"

//总风管
struct s_mainpipe {
  switchs switch_Y01;  //总风缸到总风管开关Y01状态 (状态: 1:关闭 0:打开)
  switchs switch_Y01_near;  //总风管近段开关Y01 状态 (状态: 1:关闭 0:打开)
  switchs switch_Y01_far;  //总风管远段开关Y01 状态 (状态: 1:关闭 0:打开)
  switchs switch_B02;  //总风管 裁断阀B02状态 (状态: 1:关闭 0:打开)
  switchs switch_B00;  //总风管到控制风缸 裁断阀B00状态 (状态: 1:关闭 0:打开)
  leaks leak_mainrand;   //总风管泄漏标志       0:未泄漏 1:泄漏
  float dLeakSpeed{2.};  //总风管泄漏软管泄露速度

  // Tc car
  switchs switch_W01;   //车钩阀状态
  switchs switch_W02;   //车钩软管
  float dW02LeakSpeed;  //车钩软管泄露速度

  float dChangelessLeakSpeed{1.5};  //总风管固定泄漏速度    单位:L(常压)/min

  float dCubage{30.};                //总风管容积
  float dCarToCarMRLeakSpeed{200.};  //总风管漏风量L
  float dPressure{6.};               //总风管压强
  float dMainRandChangeSpeed;
  bool bMainRandLowPressure1Status;
  float dFillInCubage{0.};  //总风缸向总风管充气 气压值

  s_mainpipe();
  void init(param_tab e);
  //更新总风管气压
  void update(float dDeltaT,
              float dMainCubagePressure,
              float dMainCubageCubage,
              float dToDepositCapacityCubage);
  float GetMainCubageToMainRandCubage(void);  //总风缸向总风管充气量  单位:L
  float GetLeakCubage(float dDeltaT);  //获得DeltaT时间内的漏风量L(常压)
};
