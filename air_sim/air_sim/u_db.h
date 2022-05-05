#pragma once

#include "s_table.h"
#include "u_base.h"
#include "u_sqlite.h"

const auto table_count = 6;
const std::string name[table_count]{
    "0气路设参数表", "1气路计算表",           "2制动力空气表",
    "3空气制动力表", "4紧急制动与列车充气比", "5辅助制动与制动充气比"};

//气路表格里全存储的浮点数
struct stable {
  size_t id, r{0}, c{0};
  std::vector<float> data;
};

//读取和转换空气数据
class u_db {
  u_sqlite* u;
  void build_data();
  //将表格数据转为插值表
  void to_small(int id, small_tab& tabs);

 public:
  stable _d[table_count];  //数据
  u_db(const nstring& db_path);
  ~u_db();
  float cell(int id, int r, int c);
  float* rows(int id, int r);     //得到id表的第r行
  int max_in_col(int id, int c);  //得到id表中第c列的最大值
  int to_tab(s_tab& des);         //将数据库数据转为计算用的数据
};
