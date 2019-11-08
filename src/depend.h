#pragma once
#ifndef DEPENDEN_H
#define DEPENDEN_H
#include "Definitions.h"
#include "log.h"
#ifdef __linux__
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <conio.h>
#include <chrono>
#include <thread>
#include <QDebug>
#include <QMessageBox>
#include <QTimer>
#include "QtGuiApplication1.h"
#include <QtWidgets/QApplication>
#endif
#define M1_Node	1
#define M2_Node	2
#define M3_Node	3
#define PI 3.1415926
#define MINUS 0.0001
//using namespace std::this_thread;     // sleep_for, sleep_until
//using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
//using std::chrono::system_clock;
#endif

class Dependency:public QObject
{
	Q_OBJECT
private:
	__int8 m_bMode;//����ģʽflag
	BOOL m_oImmediately;//VCS_MoveToPosition����
	BOOL m_oInitialisation;//�豸����flag
	BOOL m_oUpdateActive;//�Զ�ˢ��flag
	DWORD m_ulErrorCode;//����flag
	HANDLE m_KeyHandle;//�豸�˿ھ��
	WORD m_usNodeId;//�ڵ�id
	QTimer *timer=nullptr;//ˢ�����ݶ�ʱ��

	//������豸��ȡ�ĵ������
	DWORD m_ulProfileAcceleration=1000;
	DWORD m_ulProfileDeceleration=1000;
	DWORD m_ulProfileVelocity=100;

	DWORD m_ulProfileAcceleration1 = 1000;
	DWORD m_ulProfileDeceleration1 = 1000;
	DWORD m_ulProfileVelocity1 = 100;

	DWORD m_ulProfileAcceleration2 = 1000;
	DWORD m_ulProfileDeceleration2 = 1000;
	DWORD m_ulProfileVelocity2 = 100;

	DWORD m_ulProfileAcceleration3 = 1000;
	DWORD m_ulProfileDeceleration3 = 1000;
	DWORD m_ulProfileVelocity3 = 100;
	//�豸����������ֵ
	DWORD m_Amaxvelocity;
	DWORD m_AmaxAcceleration;
	DWORD m_AmaxFollowingError;

	DWORD m_Amaxvelocity1;
	DWORD m_AmaxAcceleration1;
	DWORD m_AmaxFollowingError1;

	DWORD m_Amaxvelocity2;
	DWORD m_AmaxAcceleration2;
	DWORD m_AmaxFollowingError2;

	DWORD m_Amaxvelocity3;
	DWORD m_AmaxAcceleration3;
	DWORD m_AmaxFollowingError3;
public:
	BOOL OpenDevice();//Opens communication interface
	BOOL RestoreEPOS();//Restores the old settings, stops the timer and delete the key handle
	BOOL ShowErrorInformation(DWORD p_ulErrorCode);//Shows a message box with error description of the input error code
	//void OnTimer();//��Ϊʹ����QTimer���˺�����ʱ����ʵ��
	void StopTimer();//Stops timer. Status will be displayed as disconnected
	void UpdateNodeIdString();//Converts node id to string
public slots:
	//Position Mode
	//void PositionModeInit(WORD node, DWORD pos,  DWORD ProfileVelocity ,DWORD ProfileAcceleration, DWORD ProfileDeceleration);
	//operation
	void Disable();
	void Enable();
	void Move();
	void Halt();
	void Setting();
	//value update
	void IDupdate(WORD id);
	void Posupdate(long pos);
	void RadioRelative();
	void RadioAbsolute();
	BOOL UpdateStatus();//������ʾ
	//speed mode
	void SpeedModeStart1(long speed, short threshold_current);
	void SpeedModeStart2(long speed, short threshold_current);
	void SpeedModeStart3(long speed, short threshold_current);
	void SpeedModeHalt1();
	void SpeedModeHalt2();
	void SpeedModeHalt3();
	//motion parameter(�ٶ�ģʽ�»��ض��豸��������ĳЩ����;���º���ֻ��ֱ�Ӹ�������������ֵ)
	void MotionPara1(DWORD ProfileVelocity,DWORD ProfileAcceleration,DWORD ProfileDeceleration,DWORD Amaxvelocity,DWORD AmaxAcceleration,DWORD AmaxFollowingError);
	void MotionPara2(DWORD ProfileVelocity, DWORD ProfileAcceleration, DWORD ProfileDeceleration, DWORD Amaxvelocity, DWORD AmaxAcceleration, DWORD AmaxFollowingError);
	void MotionPara3(DWORD ProfileVelocity, DWORD ProfileAcceleration, DWORD ProfileDeceleration, DWORD Amaxvelocity, DWORD AmaxAcceleration, DWORD AmaxFollowingError);
	//Teach
	void Node1Move(DWORD pos);
	void Node2Move(DWORD pos);
	void Node3Move(DWORD pos);
	void InTeachPage();//���Ϊture������UpdateStatus�������ݸ���
	void NoInTeachPage();
	void TeachReadPos();
	void TeachRecurring(long node1_pos, long node2_pos, long node3_pos);
	void TeachHalt();
public:
	Dependency();
	~Dependency();
	QString m_strActiveMode;//����ģʽ����
	QString m_strNodeId;//�ڵ�id����
	int     m_oRadio=0;//Sets state to relative or absolute movement,1 absolute 0 relative
	long    m_lActualValue;//����ʵʱλ��
	long    m_lActualValue1;
	long    m_lActualValue2;
	long    m_lActualValue3;
	long    m_lStartPosition;//��������ǰ�����λ��
	long    m_lStartPosition1;
	long    m_lStartPosition2;
	long    m_lStartPosition3;
	long    m_lTargetPosition;//����׼�������Ŀ��λ��
	long    m_lTargetPosition1;
	long    m_lTargetPosition2;
	long    m_lTargetPosition3;
	BOOL    m_TeachMode = FALSE;//�Ƿ���ʾ�̽���
	BOOL    m_Recurring = FALSE;//�Ƿ񼤻�λ�ø���
	//�ٶȲ���ģʽ
	BOOL	m_SpeedMode1=FALSE;
	BOOL	m_SpeedMode2 = FALSE;
	BOOL	m_SpeedMode3 = FALSE;
	long    m_lStartSpeed1;
	long    m_lStartSpeed2;
	long    m_lStartSpeed3;
	long    m_lActuaSpeed1;
	long    m_lActuaSpeed2;
	long    m_lActuaSpeed3;
	short   m_lActuaCurrent1;
	short   m_lActuaCurrent2;
	short   m_lActuaCurrent3;
	short   m_ThresholdCurrent1;
	short   m_ThresholdCurrent2;
	short   m_ThresholdCurrent3;
	long    m_lActuaTorgue1;
	long    m_lActuaTorgue2;
	long    m_lActuaTorgue3;//9.55f * 18 * m_Ccurrent/ m_CTargetVelocity
signals:
	void RadioSignal(BOOL radio);//1 absolute 0 relative
	void EnableSignal(BOOL enable);//1 inactivate Setting Enable, activate Disable Move Halt;0 activate Setting Enable, inactivate Disable Move Halt
	void UnconnectedSignal();
	void StartPosSignal(long Pos);
	void ActualPosSignal(long Pos);
	void OperateModeSignal(QString mod);
	//speed mode
	void SpeedSignal1(long m_lStartSpeed1, long m_lActuaSpeed1, short m_lActuaCurrent1);
	void SpeedSignal2(long m_lStartSpeed2, long m_lActuaSpeed2, short m_lActuaCurrent2);
	void SpeedSignal3(long m_lStartSpeed3, long m_lActuaSpeed3, short m_lActuaCurrent3);
	//motion parameter setting success
	void MotionParaSettingSuccessSignal1();
	void MotionParaSettingSuccessSignal2();
	void MotionParaSettingSuccessSignal3();
	//TeachMode
	void TeachModeDateSignal(long speed1, short current1, long speed2, short current2, long speed3, short current3);
	void TeachModePosSignal(long node1_pos, long node2_pos, long node3_pos);
	void RecurringDoneSignal();
};