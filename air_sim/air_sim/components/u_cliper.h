#pragma once
#pragma once
#include "../u_db.h"
#include "../u_watch.h"
#include "public_h.h"
#include "s_units.h"
#include "u_tick_c.h"
#include "u_tick_f.h"

class u_cliper {
 private:
  s_tab _t;
  s_b_record old_c;  //记录上一次的值
  s_f_record old_f;
  s_pack pack;  //数据包
  s_units obj;  //对象
  s_train_runstatus status;
  u_watch _watch;
  void to_object();
  //----------------
 public:
  void init(const string& db_path);
  void tick(float delta = 0.);
  void load(const bit_ptr mem);
  void save(bit_ptr& mem);
};
