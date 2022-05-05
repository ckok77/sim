//制动阀
#pragma once

#include "public_h.h"

struct s_brakevalve {
  //阀紧急制动继电器状态	(状态: 0:停止 1:工作 2:故障)
  works work_eb_valve{works::work};
  //阀空气制动电磁阀状态	(状态: 0:停止 1:工作 2:故障)
  works work_ep_valve{works::work};
  //(制动控制模块BCU 状态) 阀状态: 0:停止；1:工作；2:故障
  works work_valve{works::work};

  switchs switch_B07{switchs::on};  //阀B07开关常用制动切除  0:开通 1:关闭
  switchs switch_B03{switchs::on};  //阀B03开关常用制动切除  0:开通
                                    // 1:关闭//(ATO检测,机车速度用电磁阀)
  switchs valve_ease{switchs::off};  //阀空气制动制动不缓解  0:缓解  1:不缓解

  leaks leak_cylinder{
      leaks::no_leak};  // 1转向架制动缸状态   (状态: 1:泄漏 0:未泄漏)
  leaks leak_0203A{
      leaks::no_leak};  // 1转向架制动a软管状态   (状态: 1:泄漏 0:未泄漏)
  leaks leak_0203B{leaks::no_leak};

  //阀空气制动电磁阀开度状态 0-100
  bit8 ucValveMeasurementStatus{100};

  bool brake_status();         //获取制动阀状态
  bool common_brake_status();  //获取常用制动阀状态
  bool urgent_brake_status();  //获取紧急制动管路的阀状态
};
