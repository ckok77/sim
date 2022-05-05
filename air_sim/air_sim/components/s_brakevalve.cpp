#include "s_brakevalve.h"

//获取制动阀状态
bool s_brakevalve::brake_status() {
	return (is_work(work_eb_valve) || is_work(work_ep_valve));
}

//获取常用制动阀状态
bool s_brakevalve::common_brake_status() {
	return (is_work(work_valve) && is_on(switch_B07) && is_work(work_ep_valve));
}

//获取紧急制动管路的阀状态
bool s_brakevalve::urgent_brake_status() {
	return (is_work(work_valve) && is_on(switch_B07) && is_work(work_eb_valve));
}