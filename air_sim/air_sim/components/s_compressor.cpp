#include "s_compressor.h"
/*
	float pressure{ 0.f };
	float up_speed{ 0.f };
	float down_speed{ 0.f };
	float max_pressure{ 1000.f };
	float percent{ 100.f };//工作能力
*/
//空压机
s_compressor::s_compressor() {
	switch_A01 = switchs::on;   //空压机隔离塞门开放
	work_this = works::stop;
	work_aide = works::stop;

	_main = { .up_speed = 50.f, .percent = 100.f };
	_aide = { .up_speed = 7.f,.max_pressure = 8., .percent = 100.f };
}

//b_main区分是否为主压缩机
void s_compressor::init(bool b_main, param_tab e) {

	_main.up_speed = b_main ? e.v(param::主压缩机单位时间的打风量) : e.v(param::辅助空气压缩单位时间的打风量);
	//speed_supply = b_main ? e.v(param::主压缩机单位时间的打风量) : e.v(param::辅助空气压缩单位时间的打风量);
}

//设置空压机状态(通用)
void s_compressor::set_status(works ws, int wp) {

	work_this = ws;
	limit_(wp, 0, 100);
	_main.percent = wp;
	//work_percent = wp;
}

////更新空压机工作状态   获得dDeltaT(s)时间内的供风单元的供风量(单位为L),
void s_compressor::update(float dDeltaT) {
	/*

		//小于启动压力,空压机无故障,隔离塞门打开,近端供风单元,则状态为工作
		//若空压机隔离塞门开放 且空压机工作

		if (is_on(switch_A01) && is_work(work_this)) {
			// 常压下的供风体积   = 时间(换算成min)* 输出100% * 供风速度 L(常压)/min
			volume_supply = (dDeltaT / 60) * work_percent * speed_supply;
		} else	volume_supply = 0.;
	*/
	auto b = is_on(switch_A01) && is_work(work_this);
	volume_supply = b ? _main.get_volume(dDeltaT) : 0.f;
}

//辅助空压过低故障，无法启动受电弓
void s_compressor::set_status_aide(works ws) {

	//低于6.4的一个值，满足故障使用
	if (ws == works::fault)_aide.pressure = 4.0f;//设置辅助气压缸低于640kpa设置，为设置故障用
	work_aide = ws;

}
//更新辅助气压缸压强
void s_compressor::update_aider(float dDeltaT) {
	/*if (is_work(work_aide) && (pressure_aide < 8.0))pressure_aide += dDeltaT / 60 * speed_aide;*/
	if (is_work(work_aide))_aide.get_volume(dDeltaT);
}

