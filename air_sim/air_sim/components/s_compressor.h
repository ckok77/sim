#pragma once
#include "public_h.h"
#include "s_fluid.h"

//  空气压缩机
struct s_compressor {
  works work_this{works::stop};  //状态: 0:停止 1:工作 2:故障
  switchs switch_A01{
      switchs::on};  //空压机压强开关隔离塞门(状态: 1:关闭 0:打开)

  s_fluid _main, _aide;

  // int work_percent{ 1 };        //工作状态: 1-100%能力
  // float speed_supply{ 50. };  // 100%供风能力, 单位:L(常压)/min
  float volume_supply{0.};  //供风量

  works work_aide{works::stop};  //辅助空压机打风标识
  // float pressure_aide{ 7. };     //辅助气压缸压力 单位：bar
  // float speed_aide{ 0. };  //辅助空压机打风速度；

  s_compressor();
  void init(bool, param_tab e);
  void update(float dDeltaT);  //更新状空压机打风量  单位时间里完成充气量
  void set_status(works ws, int wp);  //设置空压机状态
  //受电弓专用
  void set_status_aide(works ws);
  void update_aider(float dDelta);  //更新辅助风缸压力
                                    // private
  //已知时间和供风速度 计算压力变化
  // float get_change(float delta,float spd)
};
