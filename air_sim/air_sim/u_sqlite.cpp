#include "u_sqlite.h"

u_sqlite::u_sqlite(nstring _name) {
  dbname = _name.to_string();
  auto store = make_storage(
      dbname,
      make_table("property",
                 make_column("id", &sdb::index, autoincrement(), primary_key()),
                 make_column("name", &sdb::name), make_column("type", &sdb::_t),
                 make_column("size", &sdb::_s), make_column("row", &sdb::_r),
                 make_column("col", &sdb::_c),
                 make_column("content", &sdb::content)));
  hash = store.get_all<sdb>();
}

u_sqlite::~u_sqlite() {
  hash.clear();
}

//根据字符串得到数据库中的位置
int u_sqlite::g_index(nstring name) {
  if (hash.size() <= 0)
    return -1;
  for (auto i = 0; i < hash.size(); i++) {
    if (hash[i].name == name.to_string())
      return i;
  }
  return -1;
}

//根据编号 向p中放入相应的值 调用创建p
size_t u_sqlite::g_value(int index, unsigned char*& p) {
  if (index < 0 || index >= hash.size())
    return -1;
  std::copy(hash[index].content.begin(), hash[index].content.end(), p);
  return hash[index].content.end() - hash[index].content.begin();
}

//得到相关信息
int u_sqlite::g_struct(int index, sdb& e) {
  if (index < 0 || index >= hash.size())
    return -1;
  e.index = hash[index].index;
  e._c = hash[index]._c;
  e._r = hash[index]._r;
  e._s = hash[index]._s;
  e._t = hash[index]._t;
  e.name = hash[index].name;
  return 0;
}
