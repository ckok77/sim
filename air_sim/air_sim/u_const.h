#pragma once
#include <utility>

//车辆状态常量
struct s_run_params {
  //辅助空压机空压机输出能力 (数值范围:// 1~100 )
  bit8 Assistant_AirCompressor_OutPower{100};
  //主空压机空压机输出能力  (数值范围: 1~100 )
  bit8 Main_AirCompressor_OutPower{100};
  //主风缸安全阀,排风速度 //单位：L（1个标准大气压下）/min
  float MC_SafetyValve_Speed{10.f};
  //总风管Y01开启 总风管机车间连接泄漏速度，单位：L（1个标准大气压下）/min
  float MR_Car_TO_Car_Leak_Speed{30.f};
  //总风管     一转向架 //正常固定泄漏速度，单位：L（1个标准大气压下）/min
  float MR_Changeless_Leak_Speed{0.015f};
  //常用制动管 一转向架 //正常固定泄漏速度，单位：L（1个标准大气压下）/min
  float BC_Common_Changeless_Leak_Speed{0.015f};
  //停放制动管 一转向架 //正常固定泄漏速度，单位：L（1个标准大气压下）/min
  float PK_Park_Changeless_Leak_Speed{0.015f};

  //常用制动阀空气制动电磁阀开度状态 // 0-100,初始化值
  bit8 Common_ValVe_Measurement{100};
  //空气制动缓解速度，下降1bar用的时间  单位:秒
  float Common_Release_Timer{0.6f};
  //紧急制动缓解速度，下降1bar用的时间  单位:秒
  float Exigency_Release_Timer{0.7f};
  //空气制动施加速度，增加1bar用的时间 常用制动 单位:秒
  float Force_Common_Timer{0.2f};
  //空气制动施加速度，增加1bar用的时间 快速制动  单位:秒
  float Force_Quick_Timer{0.2f};
  //空气制动施加速度，增加1bar用的时间 紧急制动 //单位:秒
  float Force_Exigency_Timer{0.1f};

  //常用制动最大级位值 逻辑数据气路的最大级位值
  float Common_Max_Grade{1000.f};
  //常用制动初始管压  单位:bar
  float Common_Brake_Pipe_Pressure{3.5f};
  //停放制动施加阀空气制动电磁阀开度状态// 0-100,初始化值
  bit8 Park_Force_ValVe_Measurement{100};
  //停放制动缓解阀空气制动电磁阀开度状态 // 0-100,初始化值
  bit8 Park_Release_ValVe_Measurement{100};
  //停放制动施加速度，增加1bar用的时间  单位:秒
  float Park_Force_Timer{1.4f};
  //停放制动缓解速度，下降1bar用的时间  单位:秒
  float Park_Release_Timer{1.2f};
  //停放制动初始管压  单位:bar
  float Park_Brake_Pipe_Pressure{0.5f};
  //停放制动施加后,常用制动反向充气时间 //单位:秒
  float ParkForce_To_CommonForce_Delay_Timer{5.f};
  //停放制动施加后,常用制动反向充气比例值 //(数值范围: 1~100 )
  bit8 ParkForce_To_CommonForce_Ratio{20};
  //常用制动制动力转换成压强参数
  float Common_BrakePower_Pressure_Parameter{
      0.10786407766990291262135922330097f};
  //常用制动压强转换成压力参数
  float Common_Pressure_BrakePower_Parameter{
      7.2535211267605633802816901408962f};
  //停放制动制动力转换成压强参数
  float Park_BrakePower_Pressure_Parameter{0.13786407766990291262135922330097f};
  //停放制动压强转换成压力参数
  float Park_Pressure_BrakePower_Parameter{7.2535211267605633802816901408962f};
  // BP制动初始管压  单位:bar
  float BP_Brake_Pipe_Pressure{6.f};
  // BP充气时间，增加1bar用的时间  单位:秒
  float BP_Brake_Pipe_Force_Timer{1.5f};
  // BP排气时间，下降1bar用的时间  单位:秒;
  float BP_Brake_Pipe_Release_Timer{0.5f};
  //辅助制动初始管压  单位:bar
  float Assise_Brake_Pipe_Pressure{0.f};
  //辅助充气时间，增加1bar用的时间  单位:秒
  float Assise_Brake_Pipe_Force_Timer{0.5f};
  //辅助排气时间，下降1bar用的时间 单位:秒
  float Assise_Brake_Pipe_Release_Timer{0.45f};
  //动车与拖车压力差  单位:bar
  float Brake_Difference_Pressure{2.f};
  //压力调节系数
  float Brake_Pressure_Parameter{0.001f};
};

//动车 拖车
enum class emu_tp { trailer = 0, powered = 2 };
//气动元件状态
enum class switchs { on = 0, off };
enum class works { stop = 0, work, fault };
enum class leaks { no_leak = 0, leak };

static inline bool is_on(switchs s) {
  return s == switchs::on;
}
static inline bool is_work(works s) {
  return s == works::work;
}
static inline bool is_leak(leaks s) {
  return s == leaks::leak;
}

//气路输入数据结构
constexpr int AirLogicIn_f = 100;
constexpr int AirLogicIn_c = 100;
#define AirLogicIn_c 100
struct LogicIn {
  float f[AirLogicIn_f];
  unsigned char c[AirLogicIn_c];
};
//气路输出数据结构
constexpr int AirLogicOut_f = 50;
constexpr int AirLogicOut_c = 100;
struct LogicOut {
  float f[AirLogicOut_f];
  unsigned char c[AirLogicOut_c];
};

template <typename T>
static inline T _max(T a, T b) {
  return a > b ? a : b;
}
template <typename T>
static inline bool in_range(T a, T _min, T _max) {
  return a >= _min && a < _max;
}
template <typename T>
static inline T limit_max(T& a, T dmax) {
  if (a > dmax)
    a = dmax;
  return a;
}
template <typename T>
static inline T limit_min(T& a, T dmin) {
  if (a < dmin)
    a = dmin;
  return a;
}
template <typename T>
static inline T limit_(T& a, T dmin, T dmax) {
  if (a < dmin)
    a = dmin;
  else if (a > dmax)
    a = dmax;
  return a;
}
//标准大气压到bar的换算
static inline float p_bar(float pressure) {
  return pressure * 1.01f;
}
static inline float bar_p(float pressure) {
  return pressure * 0.99f;
}

static inline bool g_bit(bit8 c, int bit) {
  return c & (1 << bit) ? 1 : 0;
}
static inline bool g_bits(int bit, bit_ptr p, int flag = 8) {
  int n = bit / flag;
  int b = bit % flag;
  return g_bit(p[n], b);
}
static inline bool g_bits(int bit, bit8* p, int flag = 8) {
  int n = abs(bit) / flag;
  int b = abs(bit) % flag;
  return bit > 0 ? !g_bit(p[n], b) : !g_bit(p[n], b);
}

static inline void s_bits(bit8* c, int bit, bool set) {
  int n = abs(bit) / 8;
  int b = abs(bit) % 8;
  if (bit < 0)
    set = !set;
  if (set) {
    c[n] |= (1 << b);
  } else {
    c[n] &= (~(1 << b));
  }
}
//插值条件是 x1大于x0 但y1未必大于y0 根据x2插值
static inline float interpolate(float x0,
                                float y0,
                                float x1,
                                float y1,
                                float x2) {
  if (y1 < y0)
    std::swap(y0, y1);
  return y0 + (x2 - x0) * (y1 - y0) / (x1 - x0);
}

//浮点数相等判断
static inline bool is_equal(float a, float b) {
  return int((a * 100)) == int((b * 100));
}

//渐变函数 不能用引用 因为要乘以缩放系数后进行判断
static inline float to_target(float _target, float _now, float _rate) {
  if (is_equal(_rate, 0.f) || fabs(_target - _now) < _rate)
    return _target;  //没变化率就直接赋值
  return _target > _now ? _now + _rate : _now - _rate;
}
