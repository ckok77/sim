#include "u_tick_f.h"

namespace tick_fspace {

	void record_bit(int d, bool v, s_f_record& s) {}

	//插值计算
	float LookupEvaluation(int _r, s_tab& _t, s_pack& _p) {
		auto i0 = int(_t.at.tbs[_r].value[文件表号]) - 2;//去掉参数表和计算表
		auto i1 = int(_t.at.tbs[_r].value[数据标识]);
		auto i2 = int(_t.at.tbs[_r].value[数值号]);
		auto i3 = _t.as.tbs[_r].get_count_sign();
		auto i4 = int(_t.at.tbs[_r].value[变化倍率]);
		auto fn = _p._f[i2];//当前值
		auto ft{ 0.f };//目标值
		if (abs(i4) == 0)i4 = 1;

		if (i3 == 0 && i0 > 0 && i1 > 0) {
			ft = _t.as.tbs[i0].get_v(float(i1));
			return to_target(ft, fn, i4);
		}
		if (i3 == 1) {
			auto f = _p._f[_t.as.tbs[i0].flag[0]];
			ft = _t.as.tbs[i0].get_v(f);
			return to_target(ft, fn, i4);
		}
		if (i3 == 2) {
			auto f = _p._f[_t.as.tbs[i0].flag[1]];
			ft = _t.as.tbs[i0].get_v(float(i1), f);
			return to_target(ft, fn, i4);
		}
		if (i3 == 3) {
			auto f0 = _p._f[_t.as.tbs[i0].flag[0]];
			auto f1 = _p._f[_t.as.tbs[i0].flag[1]];
			ft = _t.as.tbs[i0].get_v(f0, f1);
			return to_target(ft, fn, i4);
		}
		return 0.f;
	}

	float SubtrationCount(int _r, s_tab& _t, s_pack& _p) {
		auto f0 = _t.at.tbs[_r].value[缩放系数];
		if (int(f0) == 0)f0 = 1.f;
		auto i0 = int(_t.at.tbs[_r].value[数值号]);
		auto fn = _p._f[i0] * f0;
		if (fabs(fn) < 0.01f)return 0.f;
		auto i1 = int(_t.at.tbs[_r].value[文件表号]) - 2;//去掉参数表和计算表
		auto i2 = _t.at.tbs[_r].value[数据标识];
		auto f1 = _t.as.tbs[i1].get_r(i2);//得到变化率
		auto ft{ 0.f };//因为这是关断函数 所以目标值为0 当前值以f1的速度向0出发

		if (int(f1) == 0)return 0.f;
		return to_target(ft, fn, f1) / f0;//别忘记还有个缩放系数
	}

	float DirectnessGetValue(int _r, s_tab& _t, s_pack& _p) {
		auto fn{ 0.f };
		auto i = int(_t.at.tbs[_r].value[模拟输入]);
		if (i > 0)fn = fabs(_p.i_f[i]);
		i = int(_t.at.tbs[_r].value[关系代号]);
		if (i > 0)fn = max(fabs(_p._f[i]), fn);
		i = int(_t.at.tbs[_r].value[变化倍率]);
		auto multiple = i > 0 ? i : 0;

		auto i0 = int(_t.at.tbs[_r].value[文件表号]) - 2;//去掉参数表和计算表
		auto i1 = int(_t.at.tbs[_r].value[数据标识]);
		auto i2 = int(_t.at.tbs[_r].value[数值号]);
		auto f0{ 0.f };

		if (i0 > 0 && i1 > 0)f0 = max(LookupEvaluation(_r, _t, _p), fn);
		auto f1 = _t.at.tbs[_r].value[缩放系数];
		if (f1 < -0.01)f1 = 1 / fabs(f1);
		return f0 * f1;
	}

	//模拟量信号关断处理
	float TakeOffCourse(int _r, s_tab& _t, s_pack& _p) {
		_r = _r > 0 ? _r - 1 : 0;//取上一行数据
		auto f0 = _t.at.tbs[_r].value[变化倍率];
		auto i2 = int(_t.at.tbs[_r].value[数值号]);
		auto f2 = _t.at.tbs[_r].value[缩放系数];
		if (int(f2) == 0)f2 = 1.f;

		auto f1{ 0.f };
		if (int(f0) == 0) {
			auto i0 = int(_t.at.tbs[_r].value[文件表号]) - 2;//去掉参数表和计算表
			auto i1 = int(_t.at.tbs[_r].value[数据标识]);
			if (i0 != 0 && i1 != 0) {
				f1 = SubtrationCount(_r, _t, _p);//代入上一行数据计算
			}
		} else {
			auto fn = _p._f[i2] * f2;//
			auto ft{ 0.f };//目标为0
			f1 = to_target(f2, ft, fabs(f0));
		}
		_p._f[i2] = f1 / f2;
		return 0.f;
	}

	//参数:_r行号 _t数据库数据 _in内部的存储数据 _s上一次的数据
	float tick_f(int _r, s_tab& _t, s_pack& _p, s_f_record& _s, bool status) {
		auto i0 = int(_t.at.tbs[_r].value[数值号]);
		auto i1 = int(_t.at.tbs[_r].value[管路逻辑号]);
		auto b = i1 == 0 ? status : g_bits(i1, _p._c._data);
		auto f0{ 0.f };
		//这是什么潜规则？
		if (i0 == 0 || i0 == 100) return f0;
		//如果上个周期也是它
		if (_s.id_value == i0) {
			if (b) {//有效 则计算模拟量
				f0 = DirectnessGetValue(_r, _t, _p);
				_s.f_value = max(_s.f_value, f0);//存最大值
				_s.b_state = b;
				//最后一行了，赶快存进去
				if (_r == _t.at.tbs.size() - 1)_p._f[i0] = f0;
			}
		} else {
			if (_s.b_state) {
				_p._f[_s.id_value] = _s.f_value;
				_s.f_value = 0.f;//清除
			} else {//直接关断
				TakeOffCourse(_r, _t, _p);
			}
			if (b) {
				f0 = DirectnessGetValue(_r, _t, _p);
				_s.f_value = f0;
				_s.b_state = b;
				if (_r == _t.at.tbs.size() - 1)_p._f[i0] = f0;
			} else {
				_s.f_value = 0.f;
				_s.b_state = b;
			}
			_s.id_value = i0;
		}
		return f0;
	}
}; // namespace tick_fspace
