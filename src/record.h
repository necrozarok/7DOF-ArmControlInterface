#pragma once
#include "position.h"
class Record
{
private:
	int goal_position[3] = { 90, 90, 90 };
	Position *pos=nullptr;
	logger *log_ = nullptr;
public:
	Record(Position *pos, logger *log);
	~Record();

	void loosenGrip();
	void tightenGrip();
	void setgoal();
	void Homing();
};