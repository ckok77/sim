#pragma once
#include "public_h.h"

//-------------------------------------------------------------------------------
//主风缸
struct s_mainjar {
  //安全阀 (状态: 0:故障  1:正常)总风缸 (状态: 0:爆炸；1:正常)
  switchs switch_A05;
  switchs switch_A03;
  switchs switch_saf;                //记录安全阀的状态
  float dCubage{200.};               //总风缸容积
  float dSafetyValveOpenPower{10.};  //总风缸安全阀开启压强
  float dSafetyValveClosePower{9.};  //总风缸安全阀关闭压强
  float dSafetyValveSpeed{
      100.};  //总风缸安全阀,排风速度  单位：L（1个标准大气压下）/min
  float dPressure{10.};  //总风缸压强 (单位: bar)
  float dMainCubageChangeSpeed{0.};

  s_mainjar();
  void init(param_tab e);

  ///更新总风缸气压
  /// dMainCubageToMainRandCubage:总风缸送给主风管的空气
  void update(float dDeltaT,
              float dCompSupplyCubage,
              float dMainCubageToMainRandCubage);

  //设置故障：总风缸压强为6.0bar
  void SetMainCubageFaultPressure(switchs uc_Status);
};
