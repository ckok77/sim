/*
        包含一辆车里的所有单元
*/

#pragma once
#include "public_h.h"
#include "s_component.h"

struct s_trainunit {
  int id_car{0};
  s_unitick m_gascal{};               //制动计算
  s_footpump m_footpump{};            //脚踏泵
  s_compressor m_aider_compressor{};  //辅助空压机
  s_compressor m_air_compressor{};    //主空气压缩机
  s_mainjar m_mainjar{};              //主风缸
  s_mainpipe m_mainpipe{};            //主风管
  s_gasjar m_gasjar{};                //储气风缸
  s_brakepipe m_brakepipe{};          //制动管
  s_parking m_parking{};              //停放制动
  s_trainpipe m_trainpipe{};          // BP制动
  int i_BrakeQXN;                     //制动曲线表号
  int i_PressorQXN;                   //气压曲线表号
  int i_BPBrakeQXN;      //列车管转化制动缸气压曲线表号
  int i_AssiseBrakeQXN;  //辅助制动时列车管转化制动压力曲线表号

  //常用制动制动力转换成压强参数
  float dCommonBrakePowerToBrakePressureParameter{};
  //常用制动压强转换成压力参数
  float dCommonBrakePressureToBrakePowerParameter{};
  //停放制动制动力转换成压强参数
  float dParkBrakePowerToBrakePressureParameter{};
  //停放制动压强转换成压力参数
  float dParkBrakePressureToBrakePowerParameter{};

  //设置主风管压力
  void set_main_pressure(float dPressure);
  //设置主空压机状态
  void set_main_compressor(bit8 ucCompressorStatus);
  //设置辅助空压机状态
  void set_aide_compressor(bit8 ucCompressorStatus);
  //初始状态赋予空气系统 用于外部初始化
  void init_train_brake(param_tab& e);
  //初始化整车总风管和总风缸力压强 内部初始化
  void init_by_const(float dPressure, s_run_params st);
  float get_brake_power();
  //获取停放制动管气压值 单位:bar
  float get_pb_pipe();
  //获取EP阀电流 单位:mA
  float get_ep_current();
};
