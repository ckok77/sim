#include "s_unit.h"

//设置主风缸压力
void s_trainunit::set_main_pressure(float dPressure) {
	m_mainjar.dPressure = dPressure;
	m_mainpipe.dPressure = dPressure;
}

//根据位设置空压机的工作状态
void s_trainunit::set_main_compressor(bit8 ucCompressorStatus) {
	m_air_compressor.work_this = ucCompressorStatus & 0x01 ? works::work : works::stop;
}

//根据位设置辅助空压机的工作状态
void s_trainunit::set_aide_compressor(bit8 ucCompressorStatus) {
	m_aider_compressor.work_this =
		ucCompressorStatus & 0x01 ? works::work : works::stop;
}

//把表格中的参数导入到对象中 中间由vector<AirEquipment>转了一次
void s_trainunit::init_train_brake(param_tab& e) {

	m_footpump.init(e);
	m_aider_compressor.init(false, e);
	m_air_compressor.init(true, e);
	m_mainjar.init(e);
	m_mainpipe.init(e);
	m_gasjar.init(e);
	m_parking.init(e);
	m_trainpipe.init(e);
	m_gascal.init(e);
	id_car = int(e.v(param::车辆号));
	i_BrakeQXN = int(e.v(param::制动力到气压表号));
	i_PressorQXN = int(e.v(param::气压到制动力表号));
	i_BPBrakeQXN = int(e.v(param::紧急制动力与列车管气压));
	i_AssiseBrakeQXN = int(e.v(param::辅助制动时列车管转化制动压力曲线表号));
}

//用压力和常量值初始化结构体
void s_trainunit::init_by_const(float dPressure, s_run_params st) {
	m_mainjar.dPressure = dPressure;
	m_mainpipe.dPressure = dPressure;
	m_aider_compressor._main.percent = st.Assistant_AirCompressor_OutPower;
	m_air_compressor._main.percent = st.Main_AirCompressor_OutPower;
	m_mainjar.dSafetyValveSpeed = st.MC_SafetyValve_Speed;
	m_mainpipe.dCarToCarMRLeakSpeed = st.MR_Car_TO_Car_Leak_Speed;
	m_mainpipe.dChangelessLeakSpeed = st.MR_Changeless_Leak_Speed;
	m_gascal.dCommonChangelessLeakSpeed = st.BC_Common_Changeless_Leak_Speed;
	m_parking.dParkChangessLeakSpeed = st.PK_Park_Changeless_Leak_Speed;
	for (auto& n : m_brakepipe.valve)
		n.ucValveMeasurementStatus = st.Common_ValVe_Measurement;
	m_gascal.dReleaseCommonBrakTimer = st.Common_Release_Timer;
	m_gascal.dReleaseExigencyBrakTimer = st.Exigency_Release_Timer;

	m_gascal.dForceCommonBrakTimer = st.Force_Common_Timer;
	m_gascal.dForceQuickBrakTimer = st.Force_Quick_Timer;
	m_gascal.dForceExigencyBrakTimer = st.Force_Exigency_Timer;

	m_gascal.dCommonBrakMaxGrade = st.Common_Max_Grade;
	m_gascal.dBrakeDifferencePressure = st.Brake_Difference_Pressure;
	m_gascal.dBrakePressureParameter = st.Brake_Pressure_Parameter;

	for (auto& n : m_gascal.gs)	n.dPressure_BrakePipe = st.Common_Brake_Pipe_Pressure;

	m_parking.ucParkBrakeForceValveMeasurement = st.Park_Force_ValVe_Measurement;
	m_parking.ucParkBrakeReleaseValveMeasurement = st.Park_Release_ValVe_Measurement;
	m_parking.dParkBrakeForcecTimer = st.Park_Force_Timer;
	m_parking.dParkBrakeReleaseTimer = st.Park_Release_Timer;
	m_parking.dParkBrakePipePressure = st.Park_Brake_Pipe_Pressure;

	//停放制动施加后,常用制动反向充气时间 xxx
	st.ParkForce_To_CommonForce_Delay_Timer;
	st.ParkForce_To_CommonForce_Ratio;

	m_trainpipe.BPBrakePipePressure = st.BP_Brake_Pipe_Pressure;
	m_trainpipe.BPBrakePipeForceTimer = st.BP_Brake_Pipe_Force_Timer;
	m_trainpipe.BPBrakePipeReleaseTimer = st.BP_Brake_Pipe_Release_Timer;

	m_trainpipe.AssiseBrakePipePressure = st.Assise_Brake_Pipe_Pressure;
	m_trainpipe.AssiseBrakePipeForceTimer = st.Assise_Brake_Pipe_Force_Timer;
	m_trainpipe.AssiseBrakePipeReleaseTimer = st.Assise_Brake_Pipe_Release_Timer;

	dCommonBrakePowerToBrakePressureParameter = st.Common_BrakePower_Pressure_Parameter;
	dCommonBrakePressureToBrakePowerParameter = st.Common_Pressure_BrakePower_Parameter;

	dParkBrakePowerToBrakePressureParameter = st.Park_BrakePower_Pressure_Parameter;
	dParkBrakePressureToBrakePowerParameter = st.Park_Pressure_BrakePower_Parameter;
}

float s_trainunit::get_brake_power() { return m_gascal.dRealBrakePower; }

float s_trainunit::get_pb_pipe() { return m_parking.dParkBrakePipePressure; }

float s_trainunit::get_ep_current() { return m_gascal.dEPValueCurrent; }
