#include "s_trainpipe.h"

//-----------------------------------------------------
//列车管制动
s_trainpipe::s_trainpipe() {
  worker_b55 = works::stop;  //辅助制动阀状态        0：关闭 1：正常 2:故障
  switch_c14 = switchs::off;  //辅助制动阀隔离塞门    0：关闭 1：打开
  switch_n02 = switchs::off;  // BP制动阀缓解    1：缓解
  switch_n05 = switchs::off;  // BP制动阀施加    1：施加
}

void s_trainpipe::init(param_tab e) {
  dAssiseCubage = e.v(param::辅助风缸容积);
  dBPCubage = e.v(param::列车管容积);
  dBPRatedPressure = e.v(param::列车管最大压强);
  dAssiseRatedPressure = e.v(param::列车管最大压强);
}

float s_trainpipe::GetMainRandToBPCubage(void) {
  return dBPFillInCubage + dAssiseFillInCubage;
}

bool s_trainpipe::is_exhaust() {
  return (is_work(worker_b55) && is_on(switch_c14) && is_on(switch_n05));
}

bool s_trainpipe::is_Inflatable() {
  return (is_work(worker_b55) && is_on(switch_c14) && is_on(switch_n02));
}

void s_trainpipe::aide_when_urgent(float dDelta) {
  float f =
      dAssisePressure - dDelta / AssiseBrakePipeReleaseTimer;  //压力随时间降低
  limit_min(f, 0.f);
  dAssisePressure = f;
}

void s_trainpipe::aide_when_exhaust(float dDelta) {
  float f =
      dAssisePressure - dDelta / AssiseBrakePipeReleaseTimer;  //压力随时间降低
  limit_min(f, 0.f);
  dAssisePressure = f;
}

void s_trainpipe::aide_when_Inflatable(float dDelta, float fmain) {
  float f = dDelta / AssiseBrakePipeForceTimer;  //可以充气压
  if (f >= fmain)
    return;

  float f0 = f + dAssisePressure;  //如果高于额定压力 则等于额定压力
  limit_max(f0, dAssiseRatedPressure);
  dAssisePressure = f0;
}

//计算辅助制动气压值状态
float s_trainpipe::CountAssisePressure(float dDeltaT, float dMainPressure) {
  dAssiseFillInCubage = 0.;
  if (bExigencyBrake)
    aide_when_urgent(dDeltaT);
  else if (is_exhaust())
    aide_when_exhaust(dDeltaT);
  else if (is_Inflatable())
    aide_when_Inflatable(dDeltaT, dMainPressure);
  return dAssisePressure;
}

void s_trainpipe::bp_when_exhaust(float dDelta) {
  float f = dBPPressure - dDelta / BPBrakePipeReleaseTimer;
  limit_min(f, dBPTargetValuePressure);
  dBPPressure = f;
}

void s_trainpipe::bp_when_aide_valid(float dDelta, float aide_target) {
  if (dBPPressure <= aide_target) {  //充气
    float f0 = dDelta / BPBrakePipeForceTimer;
    float f1 = dBPPressure + f0;
    limit_max(f1, aide_target);
    dBPPressure = f1;
    dBPFillInCubage = f0 * dBPCubage;
  } else {  //排气
    float f = dBPPressure - dDelta / BPBrakePipeReleaseTimer;
    limit_min(f, aide_target);
    dBPPressure = f;
  }
}

void s_trainpipe::bp_when_aide_invalid(float dDelta, float fmain) {
  if (dBPPressure >= fmain)
    return;
  if (!is_on(switch_c14) && is_on(switch_n02)) {  //充气 没有考虑的其他情况
    float f0 = dDelta / BPBrakePipeForceTimer;
    float f1 = dBPPressure + f0;
    limit_max(f1, dBPRatedPressure);
    dBPPressure = f1;
    dBPFillInCubage = f0 * dBPCubage;
  }
}

//计算
void s_trainpipe::update(float dDeltaT,
                         float dAssiseTargetValuePressure,
                         float dMainPressure) {
  dBPFillInCubage = 0.;
  if (bExigencyBrake && is_on(switch_n05))
    bp_when_exhaust(dDeltaT);
  else if (is_work(worker_b55) && is_on(switch_c14))
    bp_when_aide_valid(dDeltaT, dAssiseTargetValuePressure);
  else
    bp_when_aide_invalid(dDeltaT, dMainPressure);
}
