#include "s_gasjar.h"

//储气风缸
s_gasjar::s_gasjar() {
  switch_B05 = switchs::on;  //储气风缸单向阀状态:0故障；1正常；
}

void s_gasjar::init(param_tab e) {
  dCubage = e.v(param::制动储气风缸容积);
  dRatingPressure = e.v(param::制动储气风缸压强);
}

//总风管向制动储气风缸充气量  单位:L
float s_gasjar::GetMainRandToDepositCapacityCubage() {
  //需要添加 设置主风管压力
  return dFillInCubage * dCubage;
}

//更新储气风缸气压
void s_gasjar::update(float dDeltaT,
                      float dMainRandPressure,
                      float dDepositCapacityToBrakePipeCubage) {
  dFillInCubage = 0;  //总风管向制动储气风缸充气 气压值

  //计算向制动充气后的压力值
  dPressure =
      (dPressure * dCubage - dDepositCapacityToBrakePipeCubage) / dCubage;
  //
  if ((dPressure < dRatingPressure) && (dPressure < dMainRandPressure) &&
      is_on(switch_B05)) {
    dFillInCubage = (dMainRandPressure + dPressure) / 2.0f - dPressure;
    if ((dPressure + dFillInCubage) > dRatingPressure) {
      dFillInCubage = dRatingPressure - dPressure;
      dPressure = dRatingPressure;
    } else {
      dPressure += dFillInCubage;
    }
  }
}