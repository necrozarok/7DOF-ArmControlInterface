#pragma once
#include "depend.h"
using namespace std;
class Position
{
private:
	logger *log_=nullptr;
	HANDLE Handle;
	DWORD ErrorCode;
	int Radio;
	BOOL Immediately;
	QString StrNodeId;


	const int min_degree[3] = { 0, 0, 0,  };
	const int max_degree[3] = { 360, 360, 360, };

	const int min_qc[4] = { 0, 0, 0,};
	const int max_qc[4] = { 4000, 4000, 4000,};

	uint16_t present_position[3] = { 0, 0, 0 };

	DWORD Node1_Acceleration=500;
	DWORD Node1_Deceleration=500;
	DWORD Node1_Velocity=500;
	DWORD Node2_Acceleration=500;
	DWORD Node2_Deceleration=500;
	DWORD Node2_Velocity=500;
	DWORD Node3_Acceleration=500;
	DWORD Node3_Deceleration=500;
	DWORD Node3_Velocity=500;
	DWORD Baudrate;
	DWORD Timeout;
	/*DWORD m_ulProfileAcceleration;
	DWORD m_ulProfileDeceleration;
	DWORD m_ulProfileVelocity;*/
	
	long   Node1_StartPos;
	long   Node2_StartPos;
	long   Node3_StartPos;

public:
	Position(logger *log);
	Position(logger *log, DWORD errorCode);
	Position(logger *log, HANDLE handle, DWORD errorCode);
	~Position();

	bool init();
	WORD calculateUnit(WORD node);
	bool Overstep(WORD node, int degree);
	bool Rotate(WORD node, int degree);
	bool On(WORD node);
	bool Off(WORD node);
	bool setAcceleration(WORD node, DWORD value);
	bool setDeceleration(WORD node, DWORD value);
	bool setVelocity(WORD node, DWORD value);

	//setting up function
	bool openPort(); //open port
	bool setBaudRate(); //set Baudrate
	void closePort(); //close communication port

	long getPresent(WORD node);
};