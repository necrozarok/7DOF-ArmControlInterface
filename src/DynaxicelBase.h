#pragma once

#ifndef DYNAMIXELBASE_H
#define DYNAMIXELBASE_H
#endif

#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include "./include/dynamixel_sdk/dynamixel_sdk.h"  
//#include "QtGuiApplication1.h"
#include <QMessageBox>
#include <QDateTime>

#ifdef __linux__
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <conio.h>
#include <chrono>
#include <thread>
#endif

// Control table address
#define ADDR_CW_POS_LIMIT			    6
#define ADDR_CCW_POS_LIMIT			    8
#define ADDR_MX_TORQUE_ENABLE           24                  // Control table address is different in Dynamixel model
#define ADDR_P_GAIN			            28
#define ADDR_MX_GOAL_POSITION           30
#define ADDR_MX_MAX_SPEED				32		
#define ADDR_MX_MAX_TORQUE				34		
#define ADDR_MX_PRESENT_POSITION        36
#define ADDR_MX_PRESENT_VELOCITY        38
#define ADDR_MX_IS_MOVING				46
#define ADDR_MX_CURRENT					68
#define ADDR_MX_TORGUE_CONTROL          70
// Data Byte Length
#define LEN_MX_GOAL_POSITION            2
#define LEN_MX_PRESENT_POSITION         2

// Protocol version
#define PROTOCOL_VERSION                1.0                 // See which protocol version is used in the Dynamixel

// Default setting
#define BAUDRATE                        57142
#define DEVICENAME                      "COM8"      

#define TORQUE_ENABLE                   1                   // Value for enabling the torque
#define TORQUE_DISABLE                  0                   // Value for disabling the torque
#define DXL_MOVING_STATUS_THRESHOLD     30                  // Dynamixel moving status threshold
//Key
#define ESC_ASCII_VALUE                 0x1b

using namespace std::this_thread;     // sleep_for, sleep_until
using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
using std::chrono::system_clock;

class DynaxicelBase
{
private:
	const int max_speed[4] = { 50, 50, 50, 100 };
	const int min_degree[4] = { 0, 5, 0, 0 };
	const int max_degree[4] = { 90, 90, 90, 90 };
	const int min_value[4] = { 1100, 150, 220, 200 };
	const int max_value[4] = { 2100, 1020, 500, 500 };
	uint16_t cw_pos_limit[4] = { 0, 290, 900, 1050 };
	uint16_t ccw_pos_limit[4] = { 4095, 2690, 4000, 3000 };
	int dxl_goal_position[4]= { 0, 0, 0, 0 };  // Goal position
	uint16_t dxl_present_position[4] = { 0, 0, 0, 0 };
	int max_torque[4] = {600,600,600,300};
	int index = 0;
	int dxl_comm_result = COMM_TX_FAIL;
	bool dxl_addparam_result = false;
	uint8_t dxl_error = 0;                          // Dynamixel error
	uint8_t param_goal_position[2];
	logger *log_ = nullptr;
public:
	//dynamixel initialization
	dynamixel::PortHandler *portHandler = dynamixel::PortHandler::getPortHandler(DEVICENAME);
	dynamixel::PacketHandler *packetHandler = dynamixel::PacketHandler::getPacketHandler(PROTOCOL_VERSION);
	dynamixel::GroupSyncWrite group = dynamixel::GroupSyncWrite(portHandler, packetHandler, ADDR_MX_GOAL_POSITION, LEN_MX_GOAL_POSITION);
public:
	DynaxicelBase();
	~DynaxicelBase();
	//��ʼ������
	void Add_Log(logger* log);
	int checkPort(void); //open port
	void closePort(void); //close communication port
	int setBaudRate(void); //���ò����ʣ���ʱ���ã�
	//��������(д��ʱ��Ҫע��ֱ���Ƶ�ַ��һ���ֽڻ��������ֽ�)
	int calculateUnit(int id);//����ÿ��λ�Ƕȶ�Ӧ�ķֱ���
	int checkDegree(int id, int degree);//����Ƿ񳬳��Ƕ�����
	int move(int id, int degree);//ָ���ؽ�ת��һ���ǶȲ�д��group
	int autoMove(int id);//�Զ��������ȡ�Ƕ�ֵ��д��group
	int move2(int id, int value);//ֱ������
	int setMaxTorque(int id, int torque); //set the torque for EACH motor
	int setSpeed(int id, int speed);//Ϊ�ؽ������ٶ�
	void setGoal(int id, int pos);//��ָ��idĿ��λ��д��Ŀ��λ������
	int enableTorque(int id); //enable torque of EACH motor
	int disableTorque(int id); //disable torque of EACH motor
	int setPgain(int id,int pgain);
	int setCW(int id ,int cw);
	int setCCW(int id, int ccw);
	//get
	UINT8 ismoving(int id);
	uint16_t getPresentCurrent(int id);
	uint16_t getPresentVelocity(int id);
	int read(int id); //read the current value of one motor
	int getGoal(int id) const;//��ȡָ��idĿ��λ�������ڵ�ֵ
	int getPresent(int id) const;//��ȡָ��id��ǰλ�������ڵ�ֵ
	int getThreshold() const;//��ȡ����ƫ��
	int getMaxTorque(int id) const;//��ȡָ��id������������ڵ�ֵ
	uint16_t getCWLimit(int id) const;
	uint16_t getCCWLimit(int id) const;
	//�������
	int loosenGrip();//����е������Ϊ��״̬
	int tightenGrip();//����е������ΪӲ״̬
	void newGoal();//��ȡÿ���ؽڵ�ǰ��λ�ò�д��Ŀ��λ������
	void AcceptPos(int i1, int i2, int i3, int i4);//��ÿ���ؽڽ�ָ��λ��д��Ŀ��λ������
	int run();//���л�е�۲�������ֱ������Ŀ��λ��
	int setMaxSpeed();//Ϊÿ���ؽ���������ٶ�
	int writeAll(void); //write all goal value into syncwrite
	void clearAll();//���ͬ��������
	void print(void); //print current and goal value of ALL motor�������ã�
	//multi get
	int ReadPosLimit();
	int * GetGroupPos();
	uint16_t *GetPresentGroupPos();
};