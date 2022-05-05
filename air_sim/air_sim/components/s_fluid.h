//用于计算的小函数
#pragma once
#include "public_h.h"

struct s_fluid {
  float pressure{0.f};
  float up_speed{0.f};
  float down_speed{0.f};
  float max_pressure{1000.f};
  float percent{100.f};  //工作能力
  //获取流量数据
  float get_volume(float delta);
};
