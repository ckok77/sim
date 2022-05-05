#include "s_mainjar.h"

//总风缸
s_mainjar::s_mainjar() {
  switch_A05 = switchs::on;  //安全阀:0故障；1正常；
  switch_A03 = switchs::on;  //总风缸:0爆炸；1正常；
  switch_saf = switchs::off;
}

void s_mainjar::init(param_tab e) {
  dCubage = e.v(param::总风管容积);
  dSafetyValveOpenPower = e.v(param::总风缸安全阀开启压强);
  dSafetyValveClosePower = e.v(param::总风缸安全阀关闭压强);
}

//辅助空压过低故障，无法启动受电弓
void s_mainjar::SetMainCubageFaultPressure(switchs uc_Status) {
  //低于6.4的一个值，满足故障使用
  if (uc_Status == switchs::off)
    dPressure = 6.0;
}

void s_mainjar::update(float dDeltaT,
                       float dCompSupplyCubage,
                       float dMainCubageToMainRandCubage) {
  if (!is_on(switch_A03)) {
    dPressure = dPressure > 10 ? dPressure - 10 : 0;
  }

  dPressure = (dPressure * dCubage - dMainCubageToMainRandCubage +
               p_bar(dCompSupplyCubage)) /
              dCubage;
  if (dPressure > dSafetyValveOpenPower)
    switch_saf = switchs::on;
  if (dPressure < dSafetyValveClosePower)
    switch_saf = switchs::off;

  if (is_on(switch_A05) && is_on(switch_saf))
    dPressure -= (dDeltaT / 60 * dSafetyValveSpeed);
}
