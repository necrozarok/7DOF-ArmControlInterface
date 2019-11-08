#pragma once
#include "QtGuiApplication1.h"
int main(int argc, char *argv[])
{
	//QMessageBox::about(NULL, "Information", "Not enough memory to allocate buffer for error information string!");
	QApplication a(argc, argv);
	DynaxicelBase Dy_Control;
	QtGuiApplication1 win;
	logger log;
	Dy_Control.Add_Log(&log);
	win.init(&Dy_Control);
	Dependency Maxon_control;
	//按钮关联
	QObject::connect(&win, &QtGuiApplication1::DisableSignal, &Maxon_control,&Dependency::Disable);
	QObject::connect(&win, &QtGuiApplication1::EnableSignal, &Maxon_control, &Dependency::Enable);
	QObject::connect(&win, &QtGuiApplication1::MoveSignal, &Maxon_control, &Dependency::Move);
	QObject::connect(&win, &QtGuiApplication1::HaltSignal, &Maxon_control, &Dependency::Halt);
	QObject::connect(&win, &QtGuiApplication1::SetSignal, &Maxon_control, &Dependency::Setting);
	QObject::connect(&win, &QtGuiApplication1::RelativeSignal, &Maxon_control, &Dependency::RadioRelative);
	QObject::connect(&win, &QtGuiApplication1::AbsoluteSignal, &Maxon_control, &Dependency::RadioAbsolute);
	QObject::connect(&win, &QtGuiApplication1::SpeedHalt1Signal, &Maxon_control, &Dependency::SpeedModeHalt1);
	QObject::connect(&win, &QtGuiApplication1::SpeedHalt1Signa2, &Maxon_control, &Dependency::SpeedModeHalt2);
	QObject::connect(&win, &QtGuiApplication1::SpeedHalt1Signa3, &Maxon_control, &Dependency::SpeedModeHalt3);
	QObject::connect(&win, &QtGuiApplication1::Node1MoveSignal, &Maxon_control, &Dependency::Node1Move);
	QObject::connect(&win, &QtGuiApplication1::Node2MoveSignal, &Maxon_control, &Dependency::Node2Move);
	QObject::connect(&win, &QtGuiApplication1::Node3MoveSignal, &Maxon_control, &Dependency::Node3Move);
	QObject::connect(&win, &QtGuiApplication1::TeachHaltSignal, &Maxon_control, &Dependency::TeachHalt);
	//数值关联win to control
	QObject::connect(&win, &QtGuiApplication1::IDSignal, &Maxon_control, &Dependency::IDupdate);
	QObject::connect(&win, &QtGuiApplication1::PosSignal, &Maxon_control, &Dependency::Posupdate);
	QObject::connect(&win, &QtGuiApplication1::StartVelocityMode1, &Maxon_control, &Dependency::SpeedModeStart1);
	QObject::connect(&win, &QtGuiApplication1::StartVelocityMode2, &Maxon_control, &Dependency::SpeedModeStart2);
	QObject::connect(&win, &QtGuiApplication1::StartVelocityMode3, &Maxon_control, &Dependency::SpeedModeStart3);
	QObject::connect(&win, &QtGuiApplication1::SendMotionPara1, &Maxon_control, &Dependency::MotionPara1);
	QObject::connect(&win, &QtGuiApplication1::SendMotionPara2, &Maxon_control, &Dependency::MotionPara2);
	QObject::connect(&win, &QtGuiApplication1::SendMotionPara3, &Maxon_control, &Dependency::MotionPara3);
	QObject::connect(&win, &QtGuiApplication1::InTeachPageSignal, &Maxon_control, &Dependency::InTeachPage);
	QObject::connect(&win, &QtGuiApplication1::NoInTeachPageSignal, &Maxon_control, &Dependency::NoInTeachPage);
	QObject::connect(&win, &QtGuiApplication1::TeachReadPosSignal, &Maxon_control, &Dependency::TeachReadPos);
	QObject::connect(&win, &QtGuiApplication1::TeachRecurringSignal, &Maxon_control, &Dependency::TeachRecurring);
	//数值关联Maxon_control to win
	QObject::connect(&Maxon_control, &Dependency::EnableSignal, &win, &QtGuiApplication1::Enable_Status);
	QObject::connect(&Maxon_control, &Dependency::UnconnectedSignal, &win, &QtGuiApplication1::Unconnected_Status);
	QObject::connect(&Maxon_control, &Dependency::RadioSignal, &win, &QtGuiApplication1::Move_Type);
	QObject::connect(&Maxon_control, &Dependency::StartPosSignal, &win, &QtGuiApplication1::Get_StartPos);
	QObject::connect(&Maxon_control, &Dependency::ActualPosSignal, &win, &QtGuiApplication1::Get_ActualPos);
	QObject::connect(&Maxon_control, &Dependency::OperateModeSignal, &win, &QtGuiApplication1::Get_OperateMode);
	QObject::connect(&Maxon_control, &Dependency::SpeedSignal1, &win, &QtGuiApplication1::VelocityData1);
	QObject::connect(&Maxon_control, &Dependency::SpeedSignal2, &win, &QtGuiApplication1::VelocityData2);
	QObject::connect(&Maxon_control, &Dependency::SpeedSignal3, &win, &QtGuiApplication1::VelocityData3);
	QObject::connect(&Maxon_control, &Dependency::MotionParaSettingSuccessSignal1, &win, &QtGuiApplication1::MotionParaSettingSuccess1);
	QObject::connect(&Maxon_control, &Dependency::MotionParaSettingSuccessSignal2, &win, &QtGuiApplication1::MotionParaSettingSuccess2);
	QObject::connect(&Maxon_control, &Dependency::MotionParaSettingSuccessSignal3, &win, &QtGuiApplication1::MotionParaSettingSuccess3);
	QObject::connect(&Maxon_control, &Dependency::TeachModeDateSignal, &win, &QtGuiApplication1::TeachModeDate);
	QObject::connect(&Maxon_control, &Dependency::TeachModePosSignal, &win, &QtGuiApplication1::Teach_Recieve_Pos);
	QObject::connect(&Maxon_control, &Dependency::RecurringDoneSignal, &win, &QtGuiApplication1::RecurringDone);
	//获取串口
	win.Dy_ComPort();
	win.show();
	return a.exec();
}


