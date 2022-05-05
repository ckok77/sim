#include "s_mainpipe.h"

s_mainpipe::s_mainpipe() {
	switch_Y01 = switchs::on;
	switch_Y01_near = switchs::on;
	switch_Y01_far = switchs::on;
	switch_B02 = switchs::on;
	switch_B00 = switchs::on;
	leak_mainrand = leaks::no_leak;
}

void s_mainpipe::init(param_tab e) {
	dCubage = e.v(param::总风管容积);
	dLeakSpeed = e.v(param::总风管单位时间漏气量);
}

//更新总风管气压 dDeltaT(s)
void s_mainpipe::update(float dDeltaT, float dMainCubagePressure, float dMainCubageCubage, float dToDepositCapacityCubage) {
	dFillInCubage = 0;  //总风缸向总风管充气 气压量  单位:L

	dPressure = (dPressure * dCubage - dToDepositCapacityCubage) / dCubage;
	if ((dMainCubageCubage > 0.1) && is_on(switch_Y01) && (dPressure < dMainCubagePressure)) {
		dFillInCubage = (dPressure + dMainCubagePressure) / 2 - dPressure;
		dPressure += dFillInCubage;
	}
	dPressure = (dPressure * dCubage - GetLeakCubage(dDeltaT) * ONE_STDAP_TO_BAR) / dCubage;
	limit_min(dPressure, 0.f);
}


//获得DeltaT时间内的漏风量L(常压) dDeltaT(s)
float s_mainpipe::GetLeakCubage(float dDeltaT) {

	float f = is_leak(leak_mainrand) && (dPressure > 0) ? dLeakSpeed : 0;//泄漏速度
	return dDeltaT * (f + dChangelessLeakSpeed) / 60;//累加在固定的泄漏速度上
}

//总风缸向总风管充气的充气量  单位:L
float s_mainpipe::GetMainCubageToMainRandCubage(void) {
	return dFillInCubage * dCubage;  //总风缸向总风管充气 气压量  单位:L
}