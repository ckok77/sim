#pragma once
#include <sqlite_orm/sqlite_orm.h>
#include <string>
#include <vector>
//#include "fmt/format.h"
//#include "third/string-utils.h"

using i32 = int_fast32_t;
using i64 = int_fast64_t;
using bit8 = unsigned char;

constexpr int tract_str_len = 40;
using tract_string = char[tract_str_len];

using blobstream = std::vector<char>;

struct nstring {
  int size;
  char* c = nullptr;
  void from_string(std::string s);
  void from_strings(std::vector<std::string>);
  std::string to_string();
  nstring();
  ~nstring();
};

//导入的类 用于回传字符串在调用者那里显示
class cmsg {
 public:
  //传入一个字符串
  virtual void __stdcall _something(nstring) = 0;
};

//属性表结构
struct sdb {
  int index;
  std::string name, class_name;    //属性名 表头(针对excel表) 归类名
  i64 _t{0}, _s{0}, _r{0}, _c{0};  // type, size, offset, row, col
  blobstream content;
};

//整列车的运行数据
struct s_train_runstatus {
  //运行模式  工况  按位处理 0位:常用制动 1位:快速制动 2位:紧急制动 3位:保持制动
  // 4辅助制动 5 耐雪制动 6 清洁制动 7 比例制动
  bool b_work_mode[10];
  float dAirBrakePower{0.f};    //空气制动力
  float dTrainBrakeGrade{0.f};  //常用制动级位
  float dTrainSpeed{0.f};       //机车当前速度

  //停放制动施加   1:施加有效信号 0:施加无有效信号
  // 0:没有施加停放制动,若原来施加,没有缓解信息是不会缓解.
  works work_park_exert;
  //停放制动缓解   1:缓解有效信号 0:缓解无有效信号
  //若缓解和施加都为1是响应施加信号
  works work_park_ease;
  s_train_runstatus() {
    work_park_exert = works::work;
    work_park_ease = works::stop;
    for (auto& b : b_work_mode)
      b = false;
  }
};

//数据包
struct s_pack {
  bit_ptr _c;  //内部数据
  usr_ptr<float> _f;
  bit8 *i_c, *o_c;  //输入输出数据
  float *i_f, *o_f;
};