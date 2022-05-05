#include "s_footpump.h"

s_footpump::s_footpump() {
	switch_A0910 = switchs::on;
	work_this = works::stop;
}

void s_footpump::init(param_tab e) {
	dSupplySpeed = e.v(param::脚踏泵单次通风量);
}

void s_footpump::update(void) {
	// 常压下的供风体积 + = 供风速度 L(常压)/次
	if (is_on(switch_A0910) && is_work(work_this)) { dTotalCubage += dSupplySpeed; }
}

