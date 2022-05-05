#pragma once
#include <sqlite_orm/sqlite_orm.h>
#include "u_base.h"

namespace so = sqlite_orm;

class u_sqlite {
 private:
  std::vector<sdb> hash;
  std::string dbname;

 public:
  u_sqlite(nstring _name);
  ~u_sqlite();
  //根据字符串得到数据库中的位置
  int g_index(nstring name);
  //根据编号 向p中放入相应的值
  size_t g_value(int index, unsigned char*& p);
  //得到相关信息
  int g_struct(int index, sdb&);
};
