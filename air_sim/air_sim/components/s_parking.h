#pragma once
#include "public_h.h"
#include "s_gasjar.h"

//停放制动
struct s_parking {
  //停放制动活动隔离塞门D 阀状态  0:故障 1:正常
  switchs switch_stopper;
  switchs switch_B12;  //停放制动隔离开关B12状态  0:开 1:关
  //第一架 1~4轴停放制动人工缓解  0:没有人工缓解 1:人工缓解
  switchs switch_manual_ease[4];
  //第一架 1轴停放制动不缓解故障  0:可以缓解 1:不可以缓解
  switchs switch_enable_ease[4];
  //停放第一架制动缸状态 然后是a和b软管   	0:正常 1:泄漏
  leaks leak_1ab[3];
  //停放第二架制动缸状态	    	0:正常 1:泄漏
  leaks leak_2ab[3];

  float dFillInCubage;  //本次计算制动缸向停放制动管填充气压值

  //停放制动施加/缓解电磁阀状态	0:失电 1:得电 2:故障
  works work_volve_exert, work_volve_ease;

  //停放制动管等效容积 1轴制动模块等效管容,BM内部管容+空气弹簧等效 体积L
  float dParkBrakePipeCubage{5.f};
  //停放制动管泄漏量L
  float dParkBrakePipeLeakCubage{0.5f};
  //停放制动管压强 bar
  float dParkBrakePipePressure{4.5f};
  //停放制动缸等效容积(一个转向架空气制动风缸=风管的体积L)
  float dParkBrakeUrnCubage{5.f};
  //停放制动缸泄漏量L
  float dParkBrakeUrnLeakCubage{0.2f};
  //第一架 1~4轴停放制动缸压强 bar
  float dParkBrakeUrn1Pressure[4]{4.5f, 4.5f, 4.5f, 4.5f};
  //停放制动施加速度，增加1bar用的时间
  float dParkBrakeForcecTimer{2.f};
  //停放制动缓解速度，下降1bar用的时间
  float dParkBrakeReleaseTimer{1.5f};
  //停放制动额定压强值
  float dParkBrakeRatingPressure{4.6f};
  //停放制动施加阀空气制动电磁阀开度状态 0-100
  bit8 ucParkBrakeForceValveMeasurement{100};
  //停放制动缓解阀空气制动电磁阀开度状态 0-100
  bit8 ucParkBrakeReleaseValveMeasurement{100};
  //停放制动管 正常固定泄漏速度，单位：L（1个标准大气压下）/min
  float dParkChangessLeakSpeed{0.6f};
  float dcParkBrakeChangeSpeed{0.f};

  s_parking();  //停放制动
  void init(param_tab e);

  void park_exhaust(float delta);
  void park_Inflatable(float delta,
                       bool b_ease,
                       const s_gasjar jar);  //缓解需要考虑储气罐是否有气

  void SetParkBrakeUrnPressure(float d_Pressure);  //停放制动缸压强 bar

  //更新停放制动 关联储气风缸 以及停放制动的施加及缓解信号
  void update(float dDeltaT, s_gasjar& jar, works exert, works ease);
  //制动储气缸向制动管和停放制动管充气量  单位:L
  float GetDepositCapacityToParkBrakePipeCubage(void);
  float GetLeakCubage(float dDeltaT);  //获取停放制动泄漏量
  bool GetParkBrakeValveStatus(void);  //获取停放制动阀和开关状态
};
