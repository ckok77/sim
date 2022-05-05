#include "s_unitick.h"

//-------------------------------------------------------------------------------
//常用制动计算
s_unitick::s_unitick() {
	//一个转向架空气制动风缸和制动缸容积
	dBrakePipeAndUrnCubage = dBrakePipeCubage + dBrakeUrnCubage;
}

void s_unitick::init(param_tab e) {
	dExigencyBrakRatingPressure = e.v(param::紧急制动压强);
	dQuickBrakRatingPressure = e.v(param::快速制动压强);
	dCommonBrakRatingPressure = e.v(param::常用制动最大级位制动压强);
	dBrakePipeCubage = e.v(param::制动管容积);
	dBrakePipeLeakCubage = e.v(param::制动管单位时间漏气量);
	dBrakeUrnCubage = e.v(param::制动缸容积);
	dBrakeUrnLeakCubage = e.v(param::制动缸单位时间漏气量);
}

//制动储气缸向制动管和制动管充气量  单位:L
float s_unitick::GetDepositCapacityToBrakePipeCubage(void) {
	return dFillInCubage * dBrakePipeAndUrnCubage;
}

//根据G制动管压力 计算电流
void s_unitick::SetEPValveCurrent(float d_BrakePipePressure) {
	float f0 = 0.f, f = d_BrakePipePressure * 100;

	if (in_range(f, 0.f, 10.f)) {
		f0 = 145.f + 4.5f * f;
	} else if (in_range(f, 10.f, 30.f)) {
		f0 = 190.f + f;
	} else if (in_range(f, 30.f, 80.f)) {
		f0 = 207.f + 0.5f * f;
	} else
		f0 = 202.2f + 0.66f * f;

	dEPValueCurrent = f0;
}

//获取泄漏量
float s_unitick::GetLeakCubage(float dDeltaT, s_brakevalve v) {
	float f{ 0.f };
	f += is_leak(v.leak_0203A) ? dBrakePipeLeakCubage : 0.f;
	f += is_leak(v.leak_0203B) ? dBrakePipeLeakCubage : 0.f;
	f += is_leak(v.leak_cylinder) ? dBrakeUrnLeakCubage : 0.f;

	//制动缸固定泄漏
	return dDeltaT * (f + dCommonChangelessLeakSpeed) / 60.f;
}

////计算紧急制动压强值 以转向架为单位
float s_unitick::CountExigencyBrakPressure(float dBrakTargetValuePressure,
	s_gasjar& jar) {
	//实际需要向制动管充气压值
	float f{ 0.f };
	//计算当前制动阀对制动管 在额定储气缸呀下的充气量.
	float f0 = dExigencyBrakRatingPressure;
	//实际需要向制动管充气压值
	f = f0 < jar.dPressure ? f0 : jar.dPressure - 0.01f;
	//动态分配气压判断
	limit_max(f, dBrakTargetValuePressure);
	limit_min(f, 0.f);

	return f;
}

//计算常用制动压强值 以转向架为单位
float s_unitick::CountQuickBrakPressure(float dBrakTargetValuePressure,
	s_gasjar& jar) {
	//实际需要向制动管充气压值
	float f{ 0.f };
	//计算当前制动阀对制动管 在额定储气缸呀下的充气量.
	float f0 = dQuickBrakRatingPressure;
	//实际需要向制动管充气压值
	f = f0 < jar.dPressure ? f0 : jar.dPressure - 0.01f;
	//动态分配气压判断
	limit_max(f, dBrakTargetValuePressure);
	limit_min(f, 0.f);
	//
	return f;
}

//计算常用制动压强值 以转向架为单位 需要制动级位
float s_unitick::CountCommonBrakPressure(float dBrakTargetValuePressure,
	s_gasjar& jar, float brake_grade) {
	//实际需要向制动管充气压值
	float f{ 0. };
	float dGrade = brake_grade / dCommonBrakMaxGrade;
	//计算当前制动阀对制动管 在额定储气缸呀下的充气量
	float f0 = dBrakTargetValuePressure;

	//实际需要向制动管充气压值
	f = f0 < jar.dPressure ? f0 : jar.dPressure - 0.01f;
	//动态分配气压判断
	limit_max(f, dBrakTargetValuePressure);
	limit_min(f, 0.f);
	return f;
}

//计算制动压力 dRealInflictionPressure 实际需要向制动管充气压值
float s_unitick::CountBrakPressure(float dDeltaT, float dBrakePipePressure,
	float dRealInflictionPressure,
	s_brakevalve v) {
	//暂时使用时间变化速度控制 改变变化率
	float dt{ 0.4f };
	float f0 = dBrakePipePressure;
	// 2014.12.15
	if (in_range(f0, 0.f, 1.f))
		dt = 0.05f;
	else if (in_range(f0, 1.f, 2.f))
		dt = 0.1f;
	else if (in_range(f0, 2.f, 3.f))
		dt = 0.2f;

	//计算常用制动充气气压值
	float dValveForcePressure =
		dt * dDeltaT / dForceCommonBrakTimer * (v.ucValveMeasurementStatus / 100);
	//原来的气压值,大于当前充气气压值处理
	if (f0 > dRealInflictionPressure) {
		if (is_on(v.valve_ease)) {
			//缓解下降气压值
			float f = dDeltaT * 1 / dReleaseCommonBrakTimer;
			f0 = 0 - dRealInflictionPressure > f ? f0 - f : dRealInflictionPressure;
		}

	}  //如果原来的气压值,小于当前充气气压值处理
	else {
		if ((f0 + dValveForcePressure) > dRealInflictionPressure) {
			dFillInCubage += dRealInflictionPressure - f0;
			f0 = dRealInflictionPressure;
		} else {
			dFillInCubage += dValveForcePressure;
			f0 += dValveForcePressure;
		}
	}
	return f0;
}

//更新制动
void s_unitick::Update(float dDeltaT, s_unitick& cal, s_brakepipe& cb,
	s_gasjar& jar, s_train_runstatus& airs, int car_tp) {
	//施加制动   制动阀的开度直接影响制动缸的充气时间
	for (auto i = 0; i < 2; i++) {
		//简写
		float* f = &cal.gs[i].dPressure_BrakePipe;
		*f = CountBrakPressure(dDeltaT, *f, cal.gs[i].dGValveRealForcePressure,
			cb.valve[i]);

		if (*f > 0) {
			float _leak = GetLeakCubage(dDeltaT, cb.valve[i]);
			*f = (dBrakePipeAndUrnCubage * (*f) - _leak * 1.01f) /
				dBrakePipeAndUrnCubage;
		} else
			*f = 0.f;
		cal.gs[i].dBrakePipePressureShow = *f;
	}

	//压力值处理过程
	//辅助制动
	bool b_aide = airs.b_work_mode[4];

	//紧急制动处理
	if (airs.b_work_mode[2]) {
		for (auto i = 0; i < 2; i++) {
			float f0 = cal.gs[i].dPressure_BrakePipe;
			float f1 = cal.dExigencyBrakRatingPressure;
			if (f0 > f1) cal.gs[i].dBrakePipePressureShow = f1;
		}
	}
	//快速制动处理
	else if (airs.b_work_mode[1]) {
		for (auto i = 0; i < 2; i++) {
			float f0 = cal.gs[i].dPressure_BrakePipe;
			float f1 = cal.dQuickBrakRatingPressure;
			if (f0 > f1) cal.gs[i].dBrakePipePressureShow = f1;
		}
	}
	//动车压力显示值处理
	else {
		for (auto i = 0; i < 2; i++) {
			float f0 = cal.gs[i].dPressure_BrakePipe;
			float f1 = cal.dCommonBrakRatingPressure;
			if (f0 > f1) cal.gs[i].dBrakePipePressureShow = f1;
		}
	}
	float f_{ 0.f };
	for (auto& n : cal.gs) {
		limit_min(n.dBrakePipePressureShow, 0.f);
		f_ += n.dBrakePipePressureShow * 0.5f;
	}

	SetEPValveCurrent(f_);
}
