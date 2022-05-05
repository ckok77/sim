#pragma once
#include "public_h.h"
#include "s_unit.h"

struct s_units {
  s_trainunit list[32];
  int list_count{0};
  s_units();
  s_run_params m_pAirCarStatus;

  float dLastTimeBrakePower;  //记录上次制动力 单个转向架
  float
      dCommonBrakePowerToBrakePressureParameter;  //常用制动制动力转换成压强参数
  float dCommonBrakePressureToBrakePowerParameter;  //常用制动压强转换成压力参数

  float dParkBrakePowerToBrakePressureParameter;  //停放制动制动力转换成压强参数
  float dParkBrakePressureToBrakePowerParameter;  //停放制动压强转换成压力参数
  int i_MBTData[10];
  int i_TBTData[10];
  bool b_BrakeTestRun;

  inline s_trainunit* _list(int car_id);

  void SetMainPressure(int i_CarID, float dPressure);  //设置主风管压力
  void SetMainCompressor(int i_CarID,
                         bool ucCompressorStatus);  //设置主空压机状态
  void SetAssistantCompressor(int i_CarID,
                              bit8 ucCompressorStatus);  //设置辅助空压机状态
  void SetParkBrakeValveSStatus(
      s_train_runstatus& t);  //临时  设置停放制动阀门状态
  void InitTrainData(s_train_runstatus& s, s_tab& t);  //初始状态赋予空气系统
  void InitPressureAndStatus(
      int iCarID,
      float dPressure,
      s_run_params p_AirCarStatus);  //初始化整车总风管和总风缸力压强
  // float   GetTrainBrakePower(c_provider& p, tabel_params& t);
  // //获取整车当前制动力  单位: kN
  float GetTrainBrakePower(s_tab& p);  //获取整车当前制动力  单位: kN
  float LookupVale(s_tab& p, int i_FileID, float f_Value);  //插值计算
  float GetTrainParkBrakePower();  //获取整车当前停放制动力  单位: kN
  float GetTrainMainRandPressure(int iCarID);  //获取总风管气压值 单位:bar
  float GetTrainGValveBrakePipePressure(
      int iCarID,
      bool is_g);  //获取G/S阀 制动管气压值 单位:bar
  float GetTrainEPValveCurrent(int iCarID);
  float GetCarBrakePower(int iCarID);
  float GetTrainParkBrakePipePressure(
      int iCarID);  //获取停放制动管气压值 单位:bar
  float GetTrainParkBrakeAxletreePressure(
      int iAxletreeID,
      int iCarID);  //获取停放制动管气压值 单位:bar
  //根据目标制动力来控制压力缸压力 返回列车管目标压力
  float xcount_target_when_aide(s_tab& p, s_train_runstatus& s);
  float xcount_target_when_urgent(s_tab& p, s_train_runstatus& s);
  float xcount_target_when_quick(s_tab& p, s_train_runstatus& s);
  float xcount_target_when_common(s_tab& p, s_train_runstatus& s);
  void XCountTrainBrakePower(s_tab& p, s_train_runstatus& s);  //计算机车制动力
  //
  //数据接口,状态更新
  void UpdateStatus(float dDeltaT, s_tab& p, s_train_runstatus& s);
};
