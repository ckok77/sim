#include "s_units.h"

s_units::s_units() {
  dLastTimeBrakePower = 0.0f;
  // dCommonBrakePowerToBrakePressureParameter =
  // COMMON_BRAKEPOWER_TO_PRESSURE_PARAMETER;  //常用制动制动力转换成压强参数
  // dCommonBrakePressureToBrakePowerParameter =
  // COMMON_PRESSURE_TO_BRAKEPOWER_PARAMETER;     //常用制动压强转换成压力参数

  // dParkBrakePowerToBrakePressureParameter =
  // PARK_BRAKEPOWER_TO_PRESSURE_PARAMETER;    //停放制动制动力转换成压强参数
  // dParkBrakePressureToBrakePowerParameter =
  // PARK_PRESSURE_TO_BRAKEPOWER_PARAMETER;       //停放制动压强转换成压力参数
}

s_trainunit* s_units::_list(int car_id) {
  for (int i = 0; i < list_count; i++) {
    if (list[i].id_car == car_id)
      return &list[i];
  }
  return nullptr;
}

//设置主风管压力
void s_units::SetMainPressure(int i_CarID, float dPressure) {
  s_trainunit* u = _list(i_CarID);
  if (u == nullptr)
    return;
  u->m_mainjar.dPressure = dPressure;
  u->m_mainpipe.dPressure = dPressure;
}

//设置主空压机状态
void s_units::SetMainCompressor(int i_CarID, bool ucCompressorStatus) {
  s_trainunit* u = _list(i_CarID);
  if (u == nullptr)
    return;
  if (ucCompressorStatus)
    u->m_air_compressor.work_this = works::work;
  else
    u->m_air_compressor.work_this = works::stop;
}

void s_units::SetAssistantCompressor(int i_CarID, bit8 ucCompressorStatus) {
  s_trainunit* u = _list(i_CarID);
  if (u == nullptr)
    return;
  if (ucCompressorStatus & 0x01)
    u->m_aider_compressor.work_this = works::work;
  else
    u->m_aider_compressor.work_this = works::stop;
}

//临时  设置停放制动阀门状态
void s_units::SetParkBrakeValveSStatus(s_train_runstatus& t) {
  for (auto i = 0; i < list_count; i++) {
    list[i].m_parking.work_volve_ease = t.work_park_ease;
    list[i].m_parking.work_volve_exert = t.work_park_exert;
  }
}

//初始状态赋予空气系统
void s_units::InitTrainData(s_train_runstatus& s, s_tab& t) {
  list_count = t.ap.tbs.size();
  for (auto i = 0; i < list_count; i++) {
    list[i].init_train_brake(t.ap.tbs[i]);
  }
  s.b_work_mode[4] = true;
}

//初始化整车总风管和总风缸力压强 用于外部调整
void s_units::InitPressureAndStatus(int iCarID,
                                    float dPressure,
                                    s_run_params p_AirCarStatus) {
  s_trainunit* u = _list(iCarID);
  if (u == nullptr)
    return;
  u->init_by_const(dPressure, p_AirCarStatus);
}

//插值计算
float s_units::LookupVale(s_tab& p, int i_FileID, float f_Value) {
  float x0{0.}, y0{0.}, x1{0.}, y1{0.};
  auto fid = i_FileID - 2;  //去掉头两个文件
  return p.as.tbs[fid].get_v(f_Value);
}

//获取整车当前制动力  单位: kN
float s_units::GetTrainBrakePower(s_tab& p) {
  float f{0.};
  for (auto i = 0; i < list_count; i++) {
    float f2{0.};
    for (auto j = 0; j < 2; j++) {  // G阀和S阀各占一半
      float f = list[i].m_gascal.gs[i].dPressure_BrakePipe;
      float f0 = (LookupVale(p, list[i].i_PressorQXN, f * 100) / 100) *
                 0.5f;  //插值所得
      f += f0;          //空气制动力
      f2 += f0;
    }
    list[i].m_gascal.dRealBrakePower = f2;  // G和S的和给到那辆车的计算模块
  }
  limit_min(f, 0.f);
  return f;  //返回全部累加值
}

//获取整车当前停放制动力  单位: kN
float s_units::GetTrainParkBrakePower() {
  float f{0.};
  for (auto i = 0; i < list_count; i++) {
    f += (1.0f - list[i].m_parking.dParkBrakePipePressure);
  }
  if (f > 0.)
    return dParkBrakePressureToBrakePowerParameter * f;
  return 0.;
}

//获取总风管气压值 单位:bar
float s_units::GetTrainMainRandPressure(int iCarID) {
  s_trainunit* u = _list(iCarID);
  if (u == nullptr)
    return 0.f;
  return u->m_mainpipe.dPressure;
}

///获取G/S阀 制动管气压值 单位:bar
float s_units::GetTrainGValveBrakePipePressure(int iCarID, bool is_g) {
  s_trainunit* u = _list(iCarID);
  if (u == nullptr)
    return 0.f;
  return u->m_gascal.gs[is_g ? 0 : 1].dBrakePipePressureShow;
}

///获取EP阀电流 单位:mA
float s_units::GetTrainEPValveCurrent(int iCarID) {
  s_trainunit* u = _list(iCarID);
  if (u == nullptr)
    return 0.f;
  return u->m_gascal.dEPValueCurrent;
}

float s_units::GetCarBrakePower(int iCarID) {
  s_trainunit* u = _list(iCarID);
  if (u == nullptr)
    return 0.f;
  return u->m_gascal.dRealBrakePower;
}

//获取停放制动管气压值 单位:bar
float s_units::GetTrainParkBrakePipePressure(int iCarID) {
  s_trainunit* u = _list(iCarID);
  if (u == nullptr)
    return 0.f;
  return u->m_parking.dParkBrakePipePressure;
}

//获取停放制动管气压值 单位:bar
float s_units::GetTrainParkBrakeAxletreePressure(int iAxletreeID, int iCarID) {
  s_trainunit* u = _list(iCarID);
  if (u == nullptr || !in_range(iAxletreeID, 0, 4))
    return 0.;
  float f0 = u->m_parking.dParkBrakeRatingPressure;
  float f1 = u->m_parking.dParkBrakeUrn1Pressure[iAxletreeID];
  float f2 = f0 - 0.2f - f1;
  limit_min(f2, 0.f);
  return f2;
}

//根据目标制动力来控制压力缸压力--辅助制动时
float s_units::xcount_target_when_aide(s_tab& p, s_train_runstatus& s) {
  s_trainunit* u = _list(1);
  if (u == nullptr)
    return 0.f;
  return LookupVale(p, u->i_AssiseBrakeQXN, u->m_trainpipe.dBPPressure) / 100;
}

//根据目标制动力来控制压力缸压力--紧急时
float s_units::xcount_target_when_urgent(s_tab& p, s_train_runstatus& s) {
  s_trainunit* u = _list(1);
  if (u == nullptr)
    return 0.f;

  int bcu_cnt{0};  // bcu的有效个数
  for (auto i = 0; i < list_count; i++) {
    for (auto j = 0; j < 2; j++) {
      bool b0 = is_work(list[i].m_brakepipe.valve[j].work_valve);
      bool b1 = is_on(list[i].m_brakepipe.valve[j].switch_B07);
      if (b0 && b1)
        bcu_cnt++;
    }
  }

  float f0{0.};  //单一转向架制动力
  float f1{0.};  //单一转向架的空气压力
  if (bcu_cnt > 0) {
    f0 = s.dAirBrakePower / bcu_cnt;
    f1 = LookupVale(p, u->i_BrakeQXN, f0 * 100) / 100;
  }

  for (auto i = 0; i < list_count; i++) {
    float f2 = list[i].m_gascal.dTargerBrakePower;            //目标制动力
    float f3 = LookupVale(p, u->i_BrakeQXN, f2 * 100) / 100;  //目标制动压强
    for (auto& n : list[i].m_gascal.gs)
      n.dBrakTargetValuePressure = f3;
  }
  return LookupVale(p, u->i_BPBrakeQXN, s.dAirBrakePower) / 100;
}

//根据目标制动力来控制压力缸压力--快速制动时
float s_units::xcount_target_when_quick(s_tab& p, s_train_runstatus& s) {
  float f0{0.};  //目标制动力
  float f1{0.};  //目标压力
  s_trainunit* u = _list(1);
  if (u == nullptr)
    return 0.;

  for (auto i = 0; i < list_count; i++) {
    f0 = list[i].m_gascal.dTargerBrakePower;
    f1 = LookupVale(p, u->i_BrakeQXN, f0 * 100) / 100;

    for (auto j = 0; j < 2; j++) {
      bool b0 = is_work(list[i].m_brakepipe.valve[j].work_valve);
      bool b1 = is_on(list[i].m_brakepipe.valve[j].switch_B07);
      if (b0 && b1) {
        list[i].m_gascal.gs[j].dBrakTargetValuePressure = f1;
      } else if (!b1) {
        list[i].m_gascal.gs[j].dBrakTargetValuePressure = 0.f;
      }
      //快速制动故障
    }
  }
  return 0.f;
}

//根据目标制动力来控制压力缸压力--常用制动时
float s_units::xcount_target_when_common(s_tab& p, s_train_runstatus& s) {
  float f0{0.f};                             //目标制动力
  float f1{0.f};                             //目标压力
  float f2 = s.b_work_mode[5] ? 4.5f : 0.f;  //耐雪制动

  s_trainunit* u = _list(1);
  if (u == nullptr)
    return 0.f;

  for (auto i = 0; i < list_count; i++) {
    f0 = list[i].m_gascal.dTargerBrakePower + f2;
    f1 = LookupVale(p, u->i_BrakeQXN, f0 * 100) / 100;

    for (auto j = 0; j < 2; j++) {
      bool b0 = is_work(list[i].m_brakepipe.valve[j].work_valve);
      bool b1 = is_on(list[i].m_brakepipe.valve[j].switch_B07);
      if (b0 && b1) {
        list[i].m_gascal.gs[j].dBrakTargetValuePressure = f1;
      } else if (!b1) {
        list[i].m_gascal.gs[j].dBrakTargetValuePressure = 0.;
      }
    }
  }
  return 0.f;
}

//计算机车制动力
void s_units::XCountTrainBrakePower(s_tab& p, s_train_runstatus& s) {
  float f0{0.f};  //列车管目标压力
  auto fn = [&](int d) -> bool { return s.b_work_mode[d]; };
  if (fn(2))
    f0 = xcount_target_when_urgent(p, s);
  else if (fn(4))
    f0 = xcount_target_when_aide(p, s);
  else if (fn(1))
    xcount_target_when_quick(p, s);
  else if (fn(0))
    xcount_target_when_common(p, s);
  //叠加清洁制动
  if (fn(6)) {
    for (auto i = 0; i < list_count; i++) {
      for (auto& n : list[i].m_gascal.gs) {
        n.dGValveRealForcePressure += 0.7f;
      }
    }
  }

  for (auto i = 0; i < list_count; i++) {
    list[i].m_trainpipe.dBPTargetValuePressure = f0;
    for (auto& n : list[i].m_gascal.gs) {
      float f1 = n.dBrakTargetValuePressure;  //目标压力
      float f2 = s.dTrainBrakeGrade;          //常用制动等级
      n.dGValveRealForcePressure =
          list[i].m_gascal.CountCommonBrakPressure(f1, list[i].m_gasjar, f2);
    }
  }
}

void s_units::UpdateStatus(float dDeltaT, s_tab& p, s_train_runstatus& s) {
  ///辅助制动计算
  float aide_p{0.f};  //辅助制动气压值
  float aide_p_target{0.f};

  for (auto i = 0; i < list_count; i++) {
    //设置系统工况
    list[i].m_trainpipe.bExigencyBrake = s.b_work_mode[2];

    list[i].m_trainpipe.dAssiseFillInCubage = 0.f;  //将BP填充气压置零
    if (list[i].m_trainpipe.dAssiseCubage > 0.1) {  //辅助制动只有头车和尾车才有
      aide_p = list[i].m_trainpipe.CountAssisePressure(
          dDeltaT, list[i].m_mainpipe.dPressure);
      aide_p_target = _max<float>(aide_p_target, aide_p);
    }
  }

  ///制动计算
  for (auto i = 0; i < list_count; i++) {
    // int tp = (int)t.table[i].f(车辆类型);
    auto tp = int(p.ap.tbs[i].v(param::制动类型));
    //列车管
    list[i].m_trainpipe.dBPFillInCubage = 0.f;  //将BP填充气压置零
    list[i].m_trainpipe.update(dDeltaT, aide_p_target,
                               list[i].m_mainpipe.dPressure);
    //常用制动
    list[i].m_gascal.dFillInCubage = 0.f;  //将制动管填充气压置零
    list[i].m_gascal.Update(dDeltaT, list[i].m_gascal, list[i].m_brakepipe,
                            list[i].m_gasjar, s, tp);
    //停放制动
    list[i].m_parking.update(dDeltaT, list[i].m_gasjar, s.work_park_exert,
                             s.work_park_ease);
    //储气风缸
    if (list[i].m_gasjar.dCubage > 0.1f) {
      float jar_out_cubage{0.f};  //制动储气缸向制动管和制动管充气量  单位:L
      jar_out_cubage = list[i].m_gascal.GetDepositCapacityToBrakePipeCubage();
      jar_out_cubage +=
          list[i].m_parking.GetDepositCapacityToParkBrakePipeCubage();
      list[i].m_gasjar.update(dDeltaT, list[i].m_mainpipe.dPressure,
                              jar_out_cubage);
    }
    //计算辅助空气压机
    float compressor_p{
        0.f};  //包括辅助空气压缩和主空气压缩机  常压下dDeltaT时间的打风量
    if (list[i].m_aider_compressor._main.up_speed > 0.1f) {
      list[i].m_aider_compressor.update(dDeltaT);
      compressor_p = list[i].m_aider_compressor.volume_supply;
    }
    //计算主压缩机
    if (list[i].m_air_compressor._main.up_speed > 0.1f) {
      list[i].m_air_compressor.update(dDeltaT);
      compressor_p += list[i].m_air_compressor.volume_supply;
    }
    //总风管
    float mainpipe_to_jar{0.};  //主风管向制动储气缸充气量  单位:L
    if (list[i].m_mainpipe.dCubage > 0.1f) {
      float f0 = list[i].m_gasjar.GetMainRandToDepositCapacityCubage();
      float f1 = list[i].m_trainpipe.GetMainRandToBPCubage();
      mainpipe_to_jar = f0 + f1;  //主风管向制动储气缸充气量和列车管  单位:L
      list[i].m_mainpipe.update(dDeltaT, list[i].m_mainjar.dPressure,
                                list[i].m_mainjar.dCubage, mainpipe_to_jar);
    }
    //计算总风缸
    if (list[i].m_mainjar.dCubage > 0.1f) {
      float mainjar_to_mainpipe{0.f};  //总风缸向主风管充气量  单位:L
      mainjar_to_mainpipe =
          list[i]
              .m_mainpipe
              .GetMainCubageToMainRandCubage();  //总风缸向主风管充气量
                                                 //单位:L
      list[i].m_mainjar.update(dDeltaT, compressor_p, mainjar_to_mainpipe);
    }
  }

  //处理总风管气压 平均
  //////////////////////////////////////////////////////////////////////////
  /*
  /*下面处理总风管气压是有些粗略,主要是没有关联上Y01开关阀,当Y01开关阀关闭时
  /*需要重新计算总风管气压
  ////////////////////////////////////////////////////////////////////////*/
  float all_bp{0.f};
  for (auto i = 0; i < list_count; i++)
    all_bp += list[i].m_mainpipe.dPressure;
  all_bp /= list_count;
  for (auto i = 0; i < list_count; i++)
    list[i].m_mainpipe.dPressure = all_bp;

  float all_jar{0.f};
  int jar_cnt{0};
  for (auto i = 0; i < list_count; i++) {
    if (list[i].m_mainpipe.dCubage > 0.01f) {  //找出有主风缸的车
      all_jar += list[i].m_mainpipe.dPressure;
      jar_cnt++;
    }
  }
  if (jar_cnt > 1) {
    all_jar /= jar_cnt;
    for (auto i = 0; i < list_count; i++) {
      if (list[i].m_mainpipe.dCubage > 0.01f) {  //找出有主风缸的车
        list[i].m_mainpipe.dPressure = all_jar;
      }
    }
  }
}