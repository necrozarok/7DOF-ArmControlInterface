#pragma once
#include <QtWidgets/QMainWindow>
#include "ui_QtGuiApplication1.h"
#include "depend.h"
#include <QButtonGroup>
#include <QRadioButton>
#include <QPushButton>
#include <QDebug>
#include <QMessageBox>
#include <QTimer>
#include <QLineEdit> 
//#include <QSerialPort>        //提供访问串口的功能 
#include <QSerialPortInfo>    //提供系统中存在的串口的信息
#include "DynaxicelBase.h"
#include <QDial> 
#include <QtGlobal>//随机数
#include <QTime>
#include <QStringList>
#include <QTableView>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QThread>
#include <QLabel>
#include <QStatusBar>
#include "Running.h"
class QtGuiApplication1 : public QMainWindow
{
	Q_OBJECT
public:
	QtGuiApplication1(QWidget *parent = Q_NULLPTR);
	~QtGuiApplication1();
	void Dy_ComPort();
	void init(DynaxicelBase *ControlObject);
public slots:
	void CurrentIndexChange(int index);
	void ToSetting_Clicked();
	void ToTest_Clicked();
	void ToRecord_Clicked();
	void ToCoordinate_Clicked();
	void Disable_Clicked();
	void Enable_Clicked();
	void Move_Clicked();
	void Exit_Clicked();
	void Halt_Clicked();
	void Set_Clicked();
	void ID_changed();
	void Pos_Send();
	void Rel_Select();
	void Abs_Select();
	void Enable_Status(bool enable);
	void Unconnected_Status();
	void Move_Type(bool type);
	void Get_StartPos(long pos);
	void Get_ActualPos(long pos);
	void Get_OperateMode(QString mod);
	void Node1MotionPara_Clicked();
	void Node2MotionPara_Clicked();
	void Node3MotionPara_Clicked();
	//Maxon Velocity Mode
	void SpeedMove1_Clicked();
	void SpeedMove2_Clicked();
	void SpeedMove3_Clicked();
	void VelocityData1(long m_lStartSpeed1, long m_lActuaSpeed1, short m_lActuaCurrent1);
	void VelocityData2(long m_lStartSpeed1, long m_lActuaSpeed1, short m_lActuaCurrent1);
	void VelocityData3(long m_lStartSpeed1, long m_lActuaSpeed1, short m_lActuaCurrent1);
	void SpeedHalf1Clecked();
	void SpeedHalf2Clecked();
	void SpeedHalf3Clecked();
	//Maxon Motion Para Setting result
	void MotionParaSettingSuccess1();
	void MotionParaSettingSuccess2();
	void MotionParaSettingSuccess3();
	
	//dynamixel
	void Connect_clicked();
	void ID1_move(int value);
	void ID2_move(int value);
	void ID3_move(int value);
	void ID4_move(int value);
	void movebutton();
	void ComScanTimeout();
	bool Dy_Initialize();
	void Enable_Torque_clicked();
	void Disable_Torque_clicked();
	void ID1_Para_Write_clicked();
	void ID2_Para_Write_clicked();
	void ID3_Para_Write_clicked();
	void ID4_Para_Write_clicked();
	void ID1_Torgue_Enable_clicked();
	void ID2_Torgue_Enable_clicked();
	void ID3_Torgue_Enable_clicked();
	void ID4_Torgue_Enable_clicked();
	void ID1_Torgue_Disable_clicked();
	void ID2_Torgue_Disable_clicked();
	void ID3_Torgue_Disable_clicked();
	void ID4_Torgue_Disable_clicked();
	void ID1_Move_clicked();
	void ID2_Move_clicked();
	void ID3_Move_clicked();
	void ID4_Move_clicked();
	void Move_clicked_Timer();
	//子线程
	void Emit_Pre_Pos();
	void Show_Pos_In_Running(uint16_t *pos);
	//Teach Interface
	void GetPos_clicked();
	void DeletePos_clicked();
	void Recurring_clicked();//单次重现
	void LoopRecurring_clicked();//循环重现
	void Running_Done();//后续执行(包括单次和循环)
	void Recurring_Halt_clicked();//暂停
	void Recurring_UrgencyHalt_clicked();//急停
	void Teach_Recieve_Pos(long node1_pos, long node2_pos, long node3_pos);
	//Maxon Teach
	void Node1DialMove(int pos);
	void Node2DialMove(int pos);
	void Node3DialMove(int pos);
	void TeachModeDate(long speed1, short current1, long speed2, short current2, long speed3, short current3);
	void RecurringDone();
signals:
	//maxon
	void DisableSignal();
	void EnableSignal();
	void MoveSignal();
	void HaltSignal();
	void SetSignal();
	void IDSignal(WORD i);
	void PosSignal(long i);
	void RelativeSignal();
	void AbsoluteSignal();
	void ExitSignal();
	void SendMotionPara1(DWORD ProfileVelocity, DWORD ProfileAcceleration, DWORD ProfileDeceleration, DWORD Amaxvelocity, DWORD AmaxAcceleration, DWORD AmaxFollowingError);
	void SendMotionPara2(DWORD ProfileVelocity, DWORD ProfileAcceleration, DWORD ProfileDeceleration, DWORD Amaxvelocity, DWORD AmaxAcceleration, DWORD AmaxFollowingError);
	void SendMotionPara3(DWORD ProfileVelocity, DWORD ProfileAcceleration, DWORD ProfileDeceleration, DWORD Amaxvelocity, DWORD AmaxAcceleration, DWORD AmaxFollowingError);
	void Node1MoveSignal(DWORD pos);
	void Node2MoveSignal(DWORD pos);
	void Node3MoveSignal(DWORD pos);
	//maxon teach interface signal
	void InTeachPageSignal();
	void NoInTeachPageSignal();
	void TeachReadPosSignal();
	void TeachRecurringSignal(long node1_pos, long node2_pos, long node3_pos);
	void TeachHaltSignal();
	//Velocity Mode Signals
	void StartVelocityMode1(long speed,short threshold_current);
	void StartVelocityMode2(long speed, short threshold_current);
	void StartVelocityMode3(long speed, short threshold_current);
	void SpeedHalt1Signal();
	void SpeedHalt1Signa2();
	void SpeedHalt1Signa3();
	//子线程
	void MoveStart(int *,int);
	void Send_Pre_Pos(uint16_t *pos);
	void MoveStop();
	void MaxonRecurringDoneSignal();
private:
	BOOL isLoopRecurring = FALSE;
	//ui
	Ui::QtGuiApplication1Class ui;
	QAction *menu_operate, *menu_about, *menu_help;
	int table_row = 0;
	//dynamixel
	int pre_pos = 0, pre_id = 0;
	DynaxicelBase *control=nullptr;//舵机操作类
	QTimer *timer1 = nullptr,*move_clicked_timer=nullptr;
	QStringList m_serialPortName;//保存串口列表
	QString  name;//需要操作的串口号
	qint32  baudRate;//波特率
	QThread running_thread;
	//thread
	int row_count;
	int column_count;
	int current_row;
	Running *readpos_running;
};