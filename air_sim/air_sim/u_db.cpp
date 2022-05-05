#include "u_db.h"

u_db::u_db(const nstring& db_path) {
  u = new u_sqlite(db_path);
  build_data();
}
u_db::~u_db() {
  delete u;
}

void u_db::build_data() {
  for (auto i = 0; i < table_count; i++) {
    nstring n;
    n.from_string(name[i]);
    int id = u->g_index(n);
    sdb r;
    u->g_struct(id, r);
    _d[i].r = r._r;
    _d[i].c = r._c;
    size_t ilen = r._r * r._c;
    _d[i].data.resize(static_cast<int>(ilen));
    bit8 *p, *t = new bit8[r._s];
    u->g_value(id, t);
    p = t;
    for (auto j = 0; j < ilen; j++) {
      _d[i].data.push_back(*(float*)p);
      p += sizeof(float);
    }
    delete[] t;
  }
}

float u_db::cell(int id, int r, int c) {
  if (id < 0 || id >= table_count || r < 0 || r >= _d[id].r || c < 0 ||
      c >= _d[id].c)
    return 0.;
  return _d[id].data[r * _d[id].c + c];
}

float* u_db::rows(int id, int r) {
  return in_range<int>(id, 0, table_count) &&
                 in_range<int>(r, 0, static_cast<int>(_d[id].r))
             ? &_d[id].data[r * _d[id].c]
             : nullptr;
}

int u_db::max_in_col(int id, int c) {
  int imax = 0;
  for (auto i = 0; i < _d[id].r; i++) {
    auto n = (int)_d[id].data[i * _d[id].c + c];
    if (imax < n)
      imax = n;
  }
  return imax;
}

void u_db::to_small(int id, small_tab& tabs) {
  assert(_d[id].r > 2);
  assert(_d[id].c > 2);
  tabs.id = id;
  tabs.flag[0] = int(cell(id, 0, 0));
  tabs.flag[1] = int(cell(id, 0, 1));
  tabs.flag[2] = int(cell(id, 1, 0));
  tabs.flag[3] = int(cell(id, 1, 1));
  //变化率
  tabs.ar._new(_d[id].r - 2);
  tabs.ax._new(_d[id].r - 2);  //去掉头两行参数
  for (auto i = 0; i < tabs.ar._size; i++) {
    tabs.ar[i] = cell(id, 2 + i, 1);
    tabs.ax[i] = cell(id, 2 + i, 0);
  }
  tabs.ay._new(_d[id].c - 2);  //去掉头两列
  for (auto i = 0; i < tabs.ay._size; i++) {
    tabs.ay[i] = cell(id, 0, i + 2);
  }
  auto i0 = tabs.ax._size;
  auto i1 = tabs.ay._size;
  tabs.av._new(i0 * i1);
  for (auto i = 0; i < i0; i++) {
    for (auto j = 0; j < i1; j++) {
      tabs.av[i * i1 + j] = cell(id, 2 + i, 2 + j);
    }
  }
}

//将数据库数据转为计算用的数据
int u_db::to_tab(s_tab& des) {
  //参数表
  assert(_d[0].c == 33);
  des.ap.tbs.resize(_d[0].r);
  for (auto i = 0; i < des.ap.tbs.size(); i++) {
    for (auto j = 0; j < _d[0].c; j++) {
      des.ap.tbs[i].value[j] = cell(0, i, j);
    }
  }
  //计算表
  assert(_d[1].c == 25);
  des.at.tbs.resize(_d[1].r);
  for (auto i = 0; i < des.ap.tbs.size(); i++) {
    for (auto j = 0; j < _d[0].c; j++) {
      des.ap.tbs[i].value[j] = cell(0, i, j);
    }
  }
  //插值表
  des.as.tbs.resize(table_count - 2);
  for (auto i = 2; i < table_count; i++) {
    to_small(i, des.as.tbs[i]);
  }
  return 0;
}