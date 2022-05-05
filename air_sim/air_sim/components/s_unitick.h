#pragma once

#include "public_h.h"
#include "s_brakepipe.h"
#include "s_gasjar.h"

// G和S阀参数
struct s_gs {
  //制动管压强和 制动缸压强 目标制动气压值 实际需要向制动管充气压值
  float dPressure_BrakePipe{0.}, dPressure_BrakeUrn{0.},
      dBrakePipePressureShow{0.}, dBrakTargetValuePressure{0.},
      dGValveRealForcePressure{0.};
};
//制动计算
struct s_unitick {
  //制动管等效容积一个转向架制动管等效管容,BM内部管容+空气弹簧等效 体积L）
  float dBrakePipeCubage{10.f};
  //制动管泄漏量L
  float dBrakePipeLeakCubage{25.f};
  //制动缸等效容积 (一个转向架空气制动风缸=风管的体积L)
  float dBrakeUrnCubage{10.f};
  //制动缸泄漏量L  (一个转向架的制动缸 漏气综合 体积L)
  float dBrakeUrnLeakCubage{30.f};
  //空气制动施加速度，增加1bar用的时间 常用制动
  float dForceCommonBrakTimer{1.5f};
  //空气制动施加速度，增加1bar用的时间 快速制动
  float dForceQuickBrakTimer{1.3f};
  //空气制动施加速度，增加1bar用的时间 紧急制动
  float dForceExigencyBrakTimer{1.2f};
  //空气制动缓解速度，下降1bar用的时间
  float dReleaseCommonBrakTimer{1.8f};
  //紧急制动缓解速度，下降1bar用的时间
  float dReleaseExigencyBrakTimer{1.8f};
  //本次计算制动缸向制动管填充气压值
  float dFillInCubage{0.f};
  //常用制动最大级位值
  float dCommonBrakMaxGrade{1000.f};
  //动车与拖车压力差
  float dBrakeDifferencePressure;
  //压力调节系数
  float dBrakePressureParameter;
  float dcBrakeCountChangeSpeed;
  //常用制动额定压强值(最大级位制动压强,除快速制动)
  float dCommonBrakRatingPressure{2.f};
  //快速制动额定压强值
  float dQuickBrakRatingPressure{3.5f};
  //紧急制动额定压强值
  float dExigencyBrakRatingPressure{3.4f};
  // EP阀电流
  float dEPValueCurrent{145.f};
  //目标制动力(KN)
  float dTargerBrakePower;
  float dRealBrakePower{0.f};
  //记录在没有常用制动后,施加停放制动,气表反响充气时间;
  float dRecordingCommonAndParkReboundTimer{0.f};
  //常用制动管 一转向架 正常固定泄漏速度，单位：L（1个标准大气压下）/min
  float dCommonChangelessLeakSpeed{1.2f};
  //一个转向架空气制动风缸和制动缸容积
  float dBrakePipeAndUrnCubage;
  // G阀和S阀的制动管压强和 制动缸压强
  s_gs gs[2];

  s_unitick();

  void init(param_tab e);
  //更新制动
  void Update(float dDeltaT,
              s_unitick& cal,
              s_brakepipe& cb,
              s_gasjar& jar,
              s_train_runstatus& airs,
              int car_tp);
  void SetEPValveCurrent(float d_BrakeGPipePressure);
  //制动储气缸向制动管和制动管充气量  单位:L
  float GetDepositCapacityToBrakePipeCubage(void);
  //获取泄漏量
  float GetLeakCubage(float dDeltaT, s_brakevalve v);
  float CountBrakPressure(float dDeltaT,
                          float dBrakePipePressure,
                          float dRealInflictionPressure,
                          s_brakevalve v);

  // 实际需要向制动管充气压值
  //计算常用制动压强值 以转向架为单位 需要制动级位
  float CountCommonBrakPressure(float dBrakTargetValuePressure,
                                s_gasjar& jar,
                                float brake_grade);

  //计算快速制动压强值 以转向架为单位
  float CountQuickBrakPressure(float dBrakTargetValuePressure, s_gasjar& jar);

  //计算紧急制动压强值 以转向架为单位
  float CountExigencyBrakPressure(float dBrakTargetValuePressure,
                                  s_gasjar& jar);
};
