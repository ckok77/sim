#include "s_brakepipe.h"

//常用制动管路 //常用制动管路    G阀=转向架一;S阀=转向架二.
s_brakepipe::s_brakepipe() {

	/*
		for (auto i = 0; i < 2; i++) {
			// G阀状态 // G阀状态: 0停止；1工作；2故障   (制动控制模块BCU  状态)
			valve[i].work_valve = works::work;
			// G阀B07开关常用制动切除  0:开通 1:关闭
			valve[i].switch_B07 = switchs::on;
			// G阀B03开关常用制动切除  0:开通 1:关闭  (ATO检测,机车速度用电磁阀)
			valve[i].switch_B03 = switchs::on;
			// G阀空气制动电磁阀状态	0:关闭 1:开通 2:故障
			valve[i].work_ep_valve = works::work;
			valve[i].work_eb_valve = works::work;
			valve[i].valve_ease = switchs::on;  //缓解
			// G阀空气制动电磁阀开度状态 0-100
			valve[i].ucValveMeasurementStatus = 100;
			valve[i].leak_cylinder = leaks::no_leak;
			valve[i].leak_0203A = leaks::no_leak;
			valve[i].leak_0203B = leaks::no_leak;
		}
	*/

}