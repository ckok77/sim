#include "u_cliper.h"

void u_cliper::init(const string& db_path) {
  nstring n;
  n.from_string(db_path);
  u_db a(n);  //局部变量 用后即焚

  //将准备号的数据库数据转为计算用结构体
  a.to_tab(_t);
  //创建内部数据
  pack._c._new(_t.at.max_col(10) / 8 + 1);
  pack._f._new(_t.at.max_col(19));
}

//周期计算
void u_cliper::tick(float delta) {
  // 1.逻辑表的计算
  using namespace tick_cspace;
  using namespace tick_fspace;
  auto i0 = _t.at.tbs.size();
  for (auto i = 0; i < i0; i++) {
    auto b = tick_c(i, _t, pack, old_c);
    auto f = tick_f(i, _t, pack, old_f, b);
  }
  // 2.将计算结构表现给对象
  to_object();
  //判断是否为第一周期
  if (_watch.is_start()) {
    auto d = delta < 0.01 ? _watch.elapse() : delta;
    // 3.通过每节车目标制动力来控制制动缸压力

    // 4.对象模块更新状态
  };
  _watch.start();
}

void u_cliper::load(const bit_ptr mem) {}
void u_cliper::save(bit_ptr& mem) {}

void u_cliper::to_object() {
  float e = 0.001f;  //判断浮点数的精度

  for (auto i = 0; i < _t.at.tbs.size(); i++) {
    auto f = _t.at.tbs[i].value;
    auto inside_c = pack._c._data;
    auto inside_f = pack._f;
    // float* f = v().rows(1, i);

    int i0 = int(f[0]);    //车辆编号列(0)
    int i19 = int(f[19]);  //数值列(19)的数据
    int i10 = int(f[10]);  //位列(10)的数据
    int idx = (int)f[1];   //用于判定计算函数的标识(1)

    auto g = [&](int index) -> s_trainunit* { return &obj.list[index]; };
    auto f_work = [&]() -> works {
      return g_bits(i10, inside_c) ? works::work : works::stop;
    };
    auto f_switch = [&]() -> switchs {
      return g_bits(i10, inside_c) ? switchs::on : switchs::off;
    };
    auto f_leak = [&]() -> leaks {
      return g_bits(i10, inside_c) ? leaks::leak : leaks::no_leak;
    };

    if (idx <= 0)
      continue;
    switch (idx) {
      case 1:  //设置制动力
        if (i19 <= 0 || inside_f[i19] < e)
          continue;
        status.dAirBrakePower = inside_f[i19];
        break;
      case 2:  //常用制动级位
        if (i19 <= 0 || inside_f[i19] < e)
          continue;
        status.dTrainBrakeGrade = inside_f[i19];
        break;
      case 3:  //设置速度
        if (i19 <= 0 || inside_f[i19] < e)
          continue;
        status.dTrainSpeed = inside_f[i19];
        break;

        ///////////  工况   ///////////////////
      case 8:  //设置常用制动指令
        if (i10 <= 0)
          continue;
        status.b_work_mode[0] = g_bits(i10, inside_c);
        break;
      case 9:  //设置快速制动指令
        if (i10 <= 0)
          continue;
        status.b_work_mode[1] = g_bits(i10, inside_c);
        break;
      case 10:  //设置紧急制动指令
        if (i10 <= 0)
          continue;
        status.b_work_mode[2] = g_bits(i10, inside_c);
        break;
      case 11:  //设置停放制动施加指令
        if (i10 <= 0)
          continue;
        status.work_park_exert = f_work();
        break;
      case 12:  //设置停放制动缓解指令
        if (i10 <= 0)
          continue;
        status.work_park_ease = f_work();
        break;
      case 13:  //设置耐雪指令
        if (i10 <= 0)
          continue;
        status.b_work_mode[5] = g_bits(i10, inside_c);
        break;
      case 14:  //设置辅助制动指令
        if (i10 <= 0)
          continue;
        status.b_work_mode[4] = g_bits(i10, inside_c);
        break;
      case 15:
        break;  //设置联挂指令
                //////////  空压机状态  ////////////////
      case 16:  //设置空压机工作标志   1：工作  0：停止
        if (i10 <= 0)
          continue;
        obj.SetMainCompressor(i0, g_bits(i10, inside_c));
        break;
      case 17:  //设置空压机压强开关隔离塞门  0：关闭 1：打开
        if (i10 <= 0)
          continue;
        g(i0)->m_air_compressor.switch_A01 = f_switch();
        break;
        //////////  BP 辅助制动  ////////////////
      case 18:
        if (i19 <= 0)
          continue;
        inside_f[i19] = (float)g(i0)->m_trainpipe.dBPPressure;
        break;
      case 19:
        if (i19 <= 0)
          continue;
        inside_f[i19] = (float)g(i0)->m_trainpipe.dAssisePressure;
        break;
      case 20:  // BP制动阀状态    0：关闭 1：正常
        if (i10 <= 0)
          continue;
        g(i0)->m_trainpipe.worker_b55 = f_work();
        break;
      case 21:  //辅助制动阀开关  0：关闭 1：打开
        if (i10 <= 0)
          continue;
        g(i0)->m_trainpipe.switch_c14 = f_switch();
        break;
      case 22:  //辅助制动阀缓解    1：缓解
        if (i10 <= 0)
          continue;
        g(i0)->m_trainpipe.switch_n02 = f_switch();
        break;
      case 23:  //辅助制动阀施加    1：施加
        if (i10 <= 0)
          continue;
        g(i0)->m_trainpipe.switch_n05 = f_switch();
        break;

        //////////  脚踏泵状态  ////////////////
      case 24:  //脚踏泵压强  单位bar
        if (i19 <= 0)
          continue;
        inside_f[i19] = g(i0)->m_footpump.dTotalCubage;
        break;
      case 25:  //脚踏泵踩下  1：踩下  0：未踩下
        if (i10 <= 0)
          continue;
        g(i0)->m_footpump.work_this = f_work();
        break;
      case 26:  //脚踏泵开关隔离塞门    状态: 1:关闭 0:打开
        if (i10 <= 0)
          continue;
        g(i0)->m_footpump.switch_A0910 = f_switch();
        break;
        //////////  总风缸状态  ////////////////
      case 32:  //总风缸压强  单位bar
        if (i19 <= 0)
          continue;
        inside_f[i19] = g(i0)->m_mainjar.dPressure;
        break;
      case 33:  //安全阀 (状态: 0:故障  1:正常）
        if (i10 <= 0)
          continue;
        g(i0)->m_mainjar.switch_A05 = f_switch();
        break;
      case 34:  //总风缸 (状态: 0:爆炸；1:正常)
        if (i10 <= 0)
          continue;
        g(i0)->m_mainjar.switch_A03 = f_switch();
        break;

        //////////  总风管状态  ////////////////
      case 40:  //总风管压强  单位bar
        if (i19 <= 0)
          continue;
        inside_f[i19] = (float)g(i0)->m_mainpipe.dPressure;
        break;
      case 41:  //总风管到总风管开关Y01状态 (状态: 1:关闭 0:打开)
        if (i10 <= 0)
          continue;
        g(i0)->m_mainpipe.switch_Y01 = f_switch();
        break;
      case 42:  //总风管近段开关Y01 状态 (状态: 1:关闭 0:打开)
        if (i10 <= 0)
          continue;
        g(i0)->m_mainpipe.switch_Y01_near = f_switch();
        break;
      case 43:  //总风管远段开关Y01 状态 (状态: 1:关闭 0:打开)
        if (i10 <= 0)
          continue;
        g(i0)->m_mainpipe.switch_Y01_far = f_switch();
        break;
      case 44:  //总风管 裁断阀B02状态 (状态: 1:关闭 0:打开)
        if (i10 <= 0)
          continue;
        g(i0)->m_mainpipe.switch_B02 = f_switch();
        break;
      case 45:  //总风管到控制风缸 裁断阀B02状态 (状态: 1:关闭 0:打开)
        if (i10 <= 0)
          continue;
        g(i0)->m_mainpipe.switch_B00 = f_switch();
        break;
      case 46:  //总风管泄漏标志       0漏 1正常
        if (i10 <= 0)
          continue;
        g(i0)->m_mainpipe.leak_mainrand = f_leak();
        break;
        //////////  储气风缸状态  ////////////////
      case 48:  //储气风缸压强  单位bar
        if (i19 <= 0)
          continue;
        inside_f[i19] = (float)g(i0)->m_gasjar.dPressure;
        break;
      case 49:  //储气风缸单向阀 B05状态 (状态: 0:故障  1:正常)
        if (i10 <= 0)
          continue;
        g(i0)->m_gasjar.switch_B05 = f_switch();
        break;
        //////////  G阀状态  ////////////////
      case 52:  // G阀  制动缸压强  单位bar
        if (i19 <= 0)
          continue;
        inside_f[i10] = obj.GetTrainGValveBrakePipePressure(i0, true);
        break;
      case 53:  // G阀  G阀状态: 0停止；1工作；2故障   (制动控制模块BCU  状态)
        if (i10 <= 0)
          continue;
        g(i0)->m_brakepipe.valve[0].work_valve = f_work();
        break;
      case 54:  // G阀  阀B07开关常用制动切除  0:开通 1:关闭
        if (i10 <= 0)
          continue;
        g(i0)->m_brakepipe.valve[0].switch_B07 = f_switch();
        break;
      case 55:  // G阀  阀B03开关常用制动切除  0:开通 1:关闭
                // (ATO检测,机车速度用电磁阀)
        if (i10 <= 0)
          continue;
        g(i0)->m_brakepipe.valve[0].switch_B03 = f_switch();
        break;
      case 56:  // G阀  1转向架制动缸状态   (状态: 1:泄漏 0:未泄漏)
        if (i10 <= 0)
          continue;
        g(i0)->m_brakepipe.valve[0].leak_cylinder = f_leak();
        break;
      case 57:  // G阀  1转向架制动a软管状态 (状态: 1:泄漏 0:未泄漏)
        if (i10 <= 0)
          continue;
        g(i0)->m_brakepipe.valve[0].leak_0203A = f_leak();
        break;
      case 58:  // G阀  1转向架制动b软管状态 (状态: 1:泄漏 0:未泄漏)
        if (i10 <= 0)
          continue;
        g(i0)->m_brakepipe.valve[0].leak_0203B = f_leak();
        break;
      case 59:  // G阀  阀空气制动电磁阀状态 (状态: 0:停止 1:工作 2:故障)
        if (i10 <= 0)
          continue;
        g(i0)->m_brakepipe.valve[0].work_ep_valve = f_work();
        break;
      case 60:  // G阀  阀紧急制动继电器状态 (状态: 0:停止 1:工作 2:故障)
        if (i10 <= 0)
          continue;
        g(i0)->m_brakepipe.valve[0].work_eb_valve = f_work();
        break;
      case 61:  // G阀  制动不缓解  0：缓解  1：不缓解
        if (i10 <= 0)
          continue;
        g(i0)->m_brakepipe.valve[0].valve_ease = f_switch();
        break;
        //////////  S阀状态  ////////////////
      case 64:  // S阀  制动缸压强  单位bar
        if (i19 <= 0)
          continue;
        inside_f[i19] = obj.GetTrainGValveBrakePipePressure(i0, false);
        break;
      case 65:  // S阀状态: 0停止；1工作；2故障   (制动控制模块BCU  状态)
        if (i10 <= 0)
          continue;
        g(i0)->m_brakepipe.valve[1].work_valve = f_work();
        break;
      case 66:  // S阀  阀B07开关常用制动切除  0:开通 1:关闭
        if (i10 <= 0)
          continue;
        g(i0)->m_brakepipe.valve[1].switch_B07 = f_switch();
        break;
      case 67:  // S阀  阀B03开关常用制动切除  0:开通 1:关闭
                // (ATO检测,机车速度用电磁阀)
        if (i10 <= 0)
          continue;
        g(i0)->m_brakepipe.valve[1].switch_B03 = f_switch();
        break;
      case 68:  // S阀  1转向架制动缸状态   (状态: 1:泄漏 0:未泄漏)
        if (i10 <= 0)
          continue;
        g(i0)->m_brakepipe.valve[1].leak_cylinder = f_leak();
        break;
      case 69:  // S阀  2转向架制动a软管状态 (状态: 1:泄漏 0:未泄漏)
        if (i10 <= 0)
          continue;
        g(i0)->m_brakepipe.valve[1].leak_0203A = f_leak();
        break;
      case 70:  // S阀  2转向架制动b软管状态 (状态: 1:泄漏 0:未泄漏)
        if (i10 <= 0)
          continue;
        g(i0)->m_brakepipe.valve[1].leak_0203B = f_leak();
        break;
      case 71:  // S阀  阀空气制动电磁阀状态 (状态: 0:停止 1:工作 2:故障)
        if (i10 <= 0)
          continue;
        g(i0)->m_brakepipe.valve[1].work_ep_valve = f_work();
        break;
      case 72:  // S阀  阀紧急制动继电器状态 (状态: 0:停止 1:工作 2:故障)
        if (i10 <= 0)
          continue;
        g(i0)->m_brakepipe.valve[1].work_eb_valve = f_work();
        break;
      case 73:  // S阀  制动不缓解  0：缓解  1：不缓解
        if (i10 <= 0)
          continue;
        g(i0)->m_brakepipe.valve[1].valve_ease = f_switch();
        break;

        //////////  停放制动状态  ////////////////
      case 76:  //第一架  1轴停放制动缸压强  单位bar
        if (i19 <= 0)
          continue;
        inside_f[i19] = g(i0)->m_parking.dParkBrakeUrn1Pressure[0];
        break;
      case 77:  //第一架  2轴停放制动缸压强  单位bar
        if (i19 <= 0)
          continue;
        inside_f[i19] = g(i0)->m_parking.dParkBrakeUrn1Pressure[1];
        break;
      case 78:  //第二架  3轴停放制动缸压强  单位bar
        if (i19 <= 0)
          continue;
        inside_f[i19] = g(i0)->m_parking.dParkBrakeUrn1Pressure[2];
        break;
      case 79:  //第二架  4轴停放制动缸压强  单位bar
        if (i19 <= 0)
          continue;
        inside_f[i19] = g(i0)->m_parking.dParkBrakeUrn1Pressure[3];
        break;
      case 80:  //停放制动压强  单位bar
        if (i19 <= 0)
          continue;
        inside_f[i19] = obj.GetTrainParkBrakePipePressure(i0);
        break;
      case 81:  //停放制动活动隔离塞门D 阀状态  0:故障 1:正常
        if (i10 <= 0)
          continue;
        g(i0)->m_parking.switch_stopper = f_switch();
        break;
      case 82:  //停放制动隔离开关B12状态  0:开 1:关
        if (i10 <= 0)
          continue;
        g(i0)->m_parking.switch_B12 = f_switch();
        break;
      case 83:  //停放第一架制动缸状态      0:正常 1:泄漏
        if (i10 <= 0)
          continue;
        g(i0)->m_parking.leak_1ab[0] = f_leak();
        break;
      case 84:  //停放第一架制动a软管状态   0:正常 1:泄漏
        if (i10 <= 0)
          continue;
        g(i0)->m_parking.leak_1ab[1] = f_leak();
        break;
      case 85:  //停放第一架制动b软管状态   0:正常 1:泄漏
        if (i10 <= 0)
          continue;
        g(i0)->m_parking.leak_1ab[2] = f_leak();
        break;
      case 86:  //停放第二架制动缸状态      0:正常 1:泄漏
        if (i10 <= 0)
          continue;
        g(i0)->m_parking.leak_2ab[0] = f_leak();
        break;
      case 87:  //停放第二架制动a软管状态   0:正常 1:泄漏
        if (i10 <= 0)
          continue;
        g(i0)->m_parking.leak_2ab[1] = f_leak();
        break;
      case 88:  //停放第一架制动b软管状态   0:正常 1:泄漏
        if (i10 <= 0)
          continue;
        g(i0)->m_parking.leak_2ab[2] = f_leak();
        break;
      case 89:  //停放制动施加电磁阀状态 0:失电 1:得电
        if (i10 <= 0)
          continue;
        g(i0)->m_parking.work_volve_exert = f_work();
        break;
      case 90:  //停放制动缓解电磁阀状态 0:失电 1:得电
        if (i10 <= 0)
          continue;
        g(i0)->m_parking.work_volve_ease = f_work();
        break;
      case 91:  //第一架 1轴停放制动人工缓解  0:没有人工缓解 1:人工缓解
        if (i10 <= 0)
          continue;
        g(i0)->m_parking.switch_manual_ease[0] = f_switch();
        break;
      case 92:  //第一架 2轴停放制动人工缓解  0:没有人工缓解 1:人工缓解
        if (i10 <= 0)
          continue;
        g(i0)->m_parking.switch_manual_ease[1] = f_switch();
        break;
      case 93:  //第二架 3轴停放制动人工缓解  0:没有人工缓解 1:人工缓解
        if (i10 <= 0)
          continue;
        g(i0)->m_parking.switch_manual_ease[2] = f_switch();
        break;
      case 94:  //第二架 4轴停放制动人工缓解  0:没有人工缓解 1:人工缓解
        if (i10 <= 0)
          continue;
        g(i0)->m_parking.switch_manual_ease[3] = f_switch();
        break;
      case 95:  //第一架 1轴停放制动不缓解故障  0:可以缓解 1:不可以缓解
        if (i10 <= 0)
          continue;
        g(i0)->m_parking.switch_enable_ease[0] = f_switch();
        break;
      case 96:  //第一架 2轴停放制动不缓解故障  0:可以缓解 1:不可以缓解
        if (i10 <= 0)
          continue;
        g(i0)->m_parking.switch_enable_ease[1] = f_switch();
        break;
      case 97:  //第二架 3轴停放制动不缓解故障  0:可以缓解 1:不可以缓解
        if (i10 <= 0)
          continue;
        g(i0)->m_parking.switch_enable_ease[2] = f_switch();
        break;
      case 98:  //第二架 4轴停放制动不缓解故障  0:可以缓解 1:不可以缓解
        if (i10 <= 0)
          continue;
        g(i0)->m_parking.switch_enable_ease[3] = f_switch();
        break;
      case 106:  //制动实验按钮  0：不进行制动实验；1：制动实验
                 // i_i10x = (int)f_RowData[AIR_LOGIC_i10];//
                 // if (i_i10x > 0)
                 //	pAirSystem->b_BrakeTestRun = g_bits(abs(i_i10x),
                 // inside_c);
        break;
      case 110:  //受电弓辅助空压机打风标识 1：打风 0：不打风
        if (i10 <= 0)
          continue;
        g(i0)->m_aider_compressor.work_aide = f_work();
        break;
      case 111:
        if (i19 <= 0)
          continue;
        inside_f[i19] = g(i0)->m_aider_compressor._aide.pressure;
        break;
      case 112:  //受电弓辅助气压缸故障，0：触发故障  1：不触发故障
        if (i10 <= 0)
          continue;
        g(i0)->m_aider_compressor.set_status_aide(f_work());
        break;
      case 113:  //受电弓故障总风缸压力低于6.0，0：触发故障  1：不触发故障
        if (i10 <= 0)
          continue;
        g(i0)->m_mainjar.SetMainCubageFaultPressure(f_switch());
        break;
      case 115:  // EP阀电流  380A使用
        if (i19 <= 0)
          continue;
        inside_f[i19] = g(i0)->m_gascal.dEPValueCurrent;
        break;
      case 121:  //清洁制动		标动使用
        if (i10 <= 0)
          continue;
        status.b_work_mode[6] = g_bits(i10, inside_c);
        break;
      case 122:  //比例制动  标动使用
        if (i10 <= 0)
          continue;
        status.b_work_mode[7] = g_bits(i10, inside_c);
        break;
      case 201:  // 1车目标制动力
        if (i19 <= 0 || inside_f[i19] < e)
          continue;
        g(i0)->m_gascal.dTargerBrakePower = inside_f[i19];
        break;
      case 202:  // 2车目标制动力
        if (i19 <= 0 || inside_f[i19] < e)
          continue;
        g(i0)->m_gascal.dTargerBrakePower = inside_f[i19];
        break;
      case 203:  // 3车目标制动力
        if (i19 <= 0 || inside_f[i19] < e)
          continue;
        g(i0)->m_gascal.dTargerBrakePower = inside_f[i19];
        break;
      case 204:  // 4车目标制动力
        if (i19 <= 0 || inside_f[i19] < e)
          continue;
        g(i0)->m_gascal.dTargerBrakePower = inside_f[i19];
        break;
      case 205:  // 5车目标制动力
        if (i19 <= 0 || inside_f[i19] < e)
          continue;
        g(i0)->m_gascal.dTargerBrakePower = inside_f[i19];
        break;
      case 206:  // 6车目标制动力
        if (i19 <= 0 || inside_f[i19] < e)
          continue;
        g(i0)->m_gascal.dTargerBrakePower = inside_f[i19];
        break;
      case 207:  // 7车目标制动力
        if (i19 <= 0 || inside_f[i19] < e)
          continue;
        g(i0)->m_gascal.dTargerBrakePower = inside_f[i19];
        break;
      case 208:  // 8车目标制动力
        if (i19 <= 0 || inside_f[i19] < e)
          continue;
        g(i0)->m_gascal.dTargerBrakePower = inside_f[i19];
        break;
      case 209:  // 9车目标制动力
        if (i19 <= 0 || inside_f[i19] < e)
          continue;
        g(i0)->m_gascal.dTargerBrakePower = inside_f[i19];
        break;
      case 210:  // 10车目标制动力
        if (i19 <= 0 || inside_f[i19] < e)
          continue;
        g(i0)->m_gascal.dTargerBrakePower = inside_f[i19];
        break;
      case 211:  // 11车目标制动力
        if (i19 <= 0 || inside_f[i19] < e)
          continue;
        g(i0)->m_gascal.dTargerBrakePower = inside_f[i19];
        break;
      case 212:  // 12车目标制动力
        if (i19 <= 0 || inside_f[i19] < e)
          continue;
        g(i0)->m_gascal.dTargerBrakePower = inside_f[i19];
        break;
      case 213:  // 13车目标制动力
        if (i19 <= 0 || inside_f[i19] < e)
          continue;
        g(i0)->m_gascal.dTargerBrakePower = inside_f[i19];
        break;
      case 214:  // 14车目标制动力
        if (i19 <= 0 || inside_f[i19] < e)
          continue;
        g(i0)->m_gascal.dTargerBrakePower = inside_f[i19];
        break;
      case 215:  // 12车目标制动力
        if (i19 <= 0 || inside_f[i19] < e)
          continue;
        g(i0)->m_gascal.dTargerBrakePower = inside_f[i19];
        break;
      case 216:  // 16车目标制动力
        if (i19 <= 0 || inside_f[i19] < e)
          continue;
        g(i0)->m_gascal.dTargerBrakePower = inside_f[i19];
        break;
      default:
        break;
    }
  }
}
