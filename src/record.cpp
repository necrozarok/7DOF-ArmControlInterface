#include "record.h"

Record::Record(Position *pos, logger * log)
{
	this->pos = pos;
	log_ = log;
}

Record::~Record()
{
	if (pos != nullptr)
		pos = nullptr;
	if (log_ != nullptr)
		log_ = nullptr;
}

void Record::loosenGrip()
{
	pos->Off(1);
	pos->Off(2);
	pos->Off(3);
	log_->Add(logINFO) << "Loosening Grip\n" << endl;
	return;
}

void Record::tightenGrip()
{
	pos->On(1);
	pos->On(2);
	pos->On(3);
	log_->Add(logINFO) << "Tightening Grip\n" << endl;
	return;
}

void Record::setgoal()
{
	goal_position[0] = pos->getPresent(1) / pos->calculateUnit(1);
	goal_position[1] = pos->getPresent(2) / pos->calculateUnit(2);
	goal_position[2] = pos->getPresent(3) / pos->calculateUnit(3);
	log_->Add(logINFO) << "Reading in new goal\n" << endl;
}

void Record::Homing()
{
	pos->Rotate(1, goal_position[0]);
	pos->Rotate(2, goal_position[1]);
	pos->Rotate(3, goal_position[2]);
	log_->Add(logINFO) << "Homing" << endl;
}
