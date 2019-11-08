#include "Running.h"



Running::Running(QObject *parent): QObject(parent)
{
}


void Running::ReadPosInRunning(int *goalpos,int thres_)
{
	if ((abs(goalpos[0] - pre_pos[0]) < thres)&& (abs(goalpos[1] - pre_pos[1]) < thres)
		&& (abs(goalpos[2] - pre_pos[2]) < thres) && (abs(goalpos[3] - pre_pos[3]) < thres)&& maxon_done)
	{
		maxon_done = false;
		emit Done();
		return;
	}
	//QTime time = QTime::currentTime(), time2;
	thres = thres_;
	emit GetPresentSingnal();
	emit PresentPos(pre_pos);
	//do
	//{
	//	// Read goal position
	//	emit GetPresentSingnal();
	//	time2 = QTime::currentTime();
	//	QTest::qSleep(1000);
	//	emit PresentPos(pre_pos);//没有运行pre数组赋值
	//	//check if the motor has reached destination
	//} while ((abs(goalpos[0] - pre_pos[0]) > thres) || (abs(goalpos[1] - pre_pos[1]) > thres)\
	//	|| (abs(goalpos[2] - pre_pos[2]) > thres) || (abs(goalpos[3] - pre_pos[3]) > thres) || (abs(time.second() - time2.second()) < 8));
}



Running::~Running()
{
	
}

void Running::GetPresentPos(uint16_t *pre_pos_)
{
	for(int i=0;i<4;i++)
	pre_pos[i] = pre_pos_[i];
}

void Running::MaxonIsDone()
{
	maxon_done = true;
}
