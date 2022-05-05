#pragma once
#include "public_h.h"

//列车管
struct s_trainpipe {
  float BPBrakePipePressure{0.};  // BP制动初始管压  单位:bar
  float BPBrakePipeForceTimer{0.};  // BP充气时间，增加1bar用的时间  单位:秒
  float BPBrakePipeReleaseTimer{0.};  // BP排气时间，下降1bar用的时间  单位:秒
  float dBPBrakeChangeSpeed{0.};

  float AssiseBrakePipePressure{0.};  //辅助制动初始管压  单位:bar
  float AssiseBrakePipeForceTimer{0.};  //辅助充气时间，增加1bar用的时间 单位:秒
  float AssiseBrakePipeReleaseTimer{
      0.};  //辅助排气时间，下降1bar用的时间  单位:秒
  float dAssisePipeChangeSpeed{0.};

  bool bExigencyBrake{false};  //紧急制动    1：紧急制动 0：未紧急制动
  works worker_b55;  //辅助制动阀状态        0：关闭 1：正常 2:故障
  switchs switch_c14;  //辅助制动阀隔离塞门    0：关闭 1：打开
  switchs switch_n02;  // BP制动阀缓解    1：缓解
  switchs switch_n05;  // BP制动阀施加    1：施加

  float dAssiseCubage{5.};         //辅助制动容积
  float dAssiseRatedPressure{0.};  //辅助制动额定气压值
  float dAssisePressure{6.};       //辅助制动压强 (单位: bar)

  float dBPCubage{0.};               // BP容积
  float dBPRatedPressure{0.};        // BP额定气压值
  float dBPTargetValuePressure{6.};  // BP 目标制动气压值
  float dBPPressure;                 // BP压强 (单位: bar)

  float dBPFillInCubage{0.};      // BP充气气压值
  float dAssiseFillInCubage{0.};  //辅助充气气压值

  s_trainpipe();
  void init(param_tab e);
  //内部函数-------------------------------->
  //辅助风缸 紧急制动时辅助风缸压力
  void aide_when_urgent(float dDelta);  //
  //排气时的辅助风缸压力
  void aide_when_exhaust(float dDelta);
  //充气时的压力 fmain:主风管压力
  void aide_when_Inflatable(float dDelta, float fmain);

  bool is_exhaust();     //根据阀门状态判断是否排气
  bool is_Inflatable();  //根据阀门状态判断是否充气

  void bp_when_exhaust(float dDelta);
  //辅助风缸有效时的BP计算 这与辅助风缸的目标值相关
  void bp_when_aide_valid(float dDelta, float aide_target);
  //辅助风缸无效时 需要与主风管比较
  void bp_when_aide_invalid(float dDelta, float fmain);
  //--------------------------------------->

  //计算辅助制动气压值状态
  float CountAssisePressure(float dDeltaT, float dMainPressure);
  //更新状BP值状态
  void update(float dDeltaT,
              float dAssiseTargetValuePressure,
              float dMainPressure);
  float GetMainRandToBPCubage(void);  //获取冲向列车管气压
};
