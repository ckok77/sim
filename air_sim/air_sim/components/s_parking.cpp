#include "s_parking.h"

//停放制动
s_parking::s_parking() {
	//停放制动活动隔离塞门D 阀状态  0:故障 1:正常
	switch_stopper = switchs::on;
	switch_B12 = switchs::on;  //停放制动隔离开关B12状态: 0:开 1:关
	for (auto& u : switch_manual_ease) u = switchs::off;  //没有人工缓解
	for (auto& u : switch_enable_ease) u = switchs::on;  //可以缓解
	work_volve_exert = works::stop;
	work_volve_ease = works::stop;
	for (auto& u : leak_1ab) u = leaks::no_leak;
	for (auto& u : leak_2ab) u = leaks::no_leak;
}

void s_parking::init(param_tab e) {

	dParkBrakePipeCubage = e.v(param::停放制动管容积);
	dParkBrakePipeLeakCubage = e.v(param::停放制动管单位时间漏气量);
	dParkBrakeUrnCubage = e.v(param::停放制动缸容积);
	dParkBrakeUrnLeakCubage = e.v(param::停放制动缸单位时间漏气量);
	dParkBrakeRatingPressure = e.v(param::停放制动压强);
}

//停放制动缸压强 bar
void s_parking::SetParkBrakeUrnPressure(float d_Pressure) {
	// 1轴~4轴
	for (auto i = 0; i < 4; i++) {
		bool b0 = switch_enable_ease[i] == switchs::off;

		dParkBrakeUrn1Pressure[i] =
			b0 ? limit_max(dParkBrakeUrn1Pressure[i], d_Pressure) : d_Pressure;
		if (switch_manual_ease[i] == switchs::on) {
			dParkBrakeUrn1Pressure[i] = dParkBrakeRatingPressure + 0.5f;
		}
	}
}

//获取停放制动塞门和开关状态
bool s_parking::GetParkBrakeValveStatus(void) {
	return switch_stopper == switchs::on;
}

//获取停放制动泄漏量
float s_parking::GetLeakCubage(float dDeltaT) {
	float dLeakCubage = 0.;
	for (auto i = 0; i < 3; i++) {
		float f = i == 0 ? dParkBrakeUrnLeakCubage : dParkBrakePipeLeakCubage;
		if (leak_1ab[i] == leaks::leak) dLeakCubage += f;
		if (leak_2ab[i] == leaks::leak) dLeakCubage += f;
	}
	return dDeltaT / 60 * (dLeakCubage + dParkChangessLeakSpeed);
}

//制动储气缸向制动管和停放制动管充气量  单位:L
float s_parking::GetDepositCapacityToParkBrakePipeCubage(void) {
	float dParkCubage = dParkBrakePipeCubage + dParkBrakeUrnCubage * 2;
	return dFillInCubage * dParkCubage;
}

//施加停放制动 排气
void s_parking::park_exhaust(float delta) {

	float f = delta * 1 / dParkBrakeReleaseTimer;  //缓解下降气压值
	if (dParkBrakePipePressure > f)dParkBrakePipePressure -= f;
	limit_min(dParkBrakePipePressure, 0.f);
}

//无论是否缓解制动 都持续充气
void s_parking::park_Inflatable(float delta, bool b_ease, const s_gasjar jar) {

	float f0{ 0.f };
	if (b_ease)f0 = delta / dParkBrakeForcecTimer * (ucParkBrakeForceValveMeasurement / 100);
	//前者说明储气罐压力够大
	float f1 = jar.dPressure > dParkBrakeRatingPressure ? dParkBrakeRatingPressure : jar.dPressure + 0.05f;
	limit_min(f1, 0.f);
	if (dParkBrakePipePressure > f1) {//压力太高
		float f2 = delta * 1 / dParkBrakeReleaseTimer;//缓解下降气压值
		if (dParkBrakePipePressure - f2 > f1)dParkBrakePipePressure -= f2;
		limit_min(dParkBrakePipePressure, f1);
	}
	if (dParkBrakePipePressure + f0 > f1) {
		dFillInCubage += f1 - dParkBrakePipePressure;//dFillInCubage<0
		dParkBrakePipePressure = f1;
	} else {
		dFillInCubage += f0;
		dParkBrakePipePressure += f0;
	}
}

//更新停放制动
void s_parking::update(float dDeltaT, s_gasjar& jar, works exert, works ease) {

	dFillInCubage = 0;

	bool _b_volve = GetParkBrakeValveStatus() && is_on(switch_B12);//获取停放制动塞门和开关状态

	bool _b_exert = is_work(exert) && is_work(work_volve_exert);//是否施加停放制动

	bool _b_ease = is_work(ease) && is_work(work_volve_ease);//是否缓解停放制动

	if (_b_volve) {
		if (_b_exert)park_exhaust(dDeltaT);//施加停放制动=排气
		else park_Inflatable(dDeltaT, _b_ease, jar);

	} else if (!is_on(switch_B12)) {//人工缓解		 
		dParkBrakePipePressure = dParkBrakeRatingPressure + 0.5f;
	}
	limit_min(dParkBrakePipePressure, 0.f);
	SetParkBrakeUrnPressure(dParkBrakePipePressure);
}