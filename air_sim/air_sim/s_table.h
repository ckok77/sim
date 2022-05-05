//处理表格数据
#pragma once

#include <vector>
#include "u_const.h"
#include "u_ptr.h"

/*
 * flag0 flag1  y0 y1 y2
 * flag2 flag3
 * x0    rate0  v  v
 * x1    rate1  v  v
 * x2    rate2  v  v
 */
//--------------------------------------
//插值表的结构
struct small_tab {
  int id;
  usr_ptr<float> ax, ay, ar, av;
  int flag[4]{0, 0, 0, 0};  // 0:[0,0]1:[1,0]2:[0,1]3:[1,1]
  int get_count_sign();     //得到运算符
  //插值
  float get_v(int id_x, int id_y);
  // flag = 1 已知x float 固定y值（=第2列）的求值
  float get_v(float _x, int id_y = 2);
  // flag = 1 已知x float 和y float 的求值
  float get_v(float _x, float _y);
  float get_r(float _x);
};
struct small_tabs {
  std::vector<small_tab> tbs;
};

//--------------------------------------
//参数表的结构
constexpr int param_col_count = 33;  //参数表的列数常量
namespace param {
enum _ : int {
  车辆号 = 0,
  制动类型,
  制动力到气压表号,
  气压到制动力表号,
  紧急制动压强,
  停放制动压强,
  快速制动压强,
  常用制动最大级位制动压强,
  脚踏泵单次通风量,
  辅助空气压缩单位时间的打风量,
  主压缩机单位时间的打风量,
  总风缸容积,
  总风缸安全阀开启压强,
  总风缸安全阀关闭压强,
  总风管容积,
  总风管单位时间漏气量,
  制动储气风缸容积,
  制动储气风缸压强,
  制动管容积,
  制动管单位时间漏气量,
  制动缸容积,
  制动缸单位时间漏气量,
  停放制动管容积,
  停放制动管单位时间漏气量,
  停放制动缸容积,
  停放制动缸单位时间漏气量,
  辅助风缸容积,
  列车管容积,
  列车管最大压强,
  紧急制动力与列车管气压,
  辅助制动时列车管转化制动压力曲线表号,
  紧急制动力到列车管压力表号,
  耐雪制动,
  编号
};
};
struct param_tab {
  float value[param_col_count];
  float v(int index) { return value[index]; }
};
struct param_tabs {
  std::vector<param_tab> tbs;
};

//--------------------------------------
//计算表的结构
constexpr int tick_col_count = 25;  //计算表的列数常量
enum ticks : int {
  车辆号 = 0,
  定义号,
  设定初值,
  输入阀1,
  输入阀2,
  输入阀3,
  输入阀4,
  关联号1,
  关联号2,
  关联号3,
  管路逻辑号,
  模拟输入,
  关系代号,
  时间,
  设定值,
  变化倍率,
  数据标识,
  文件表号,
  缩放系数,
  数值号,
  位输出,
  值输出,
  控制目的,
  备注,
  编号
};

struct tick_tab {
  float value[tick_col_count];
};
struct tick_tabs {
  std::vector<tick_tab> tbs;
  int max_col(int c);
};
//--------------------------------------
//表格数据都放进来
struct s_tab {
  param_tabs ap;
  tick_tabs at;
  small_tabs as;
};
