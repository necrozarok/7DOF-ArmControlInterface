#ifndef RUNNING_H
#define RUNNING_H
#pragma once
#include <QThread>
#include <QMutex>
#include <QObject>
#include <QTest> 
#include "DynaxicelBase.h"
extern int globlIndex;
extern QMutex globlMutex;
class Running :public QObject
{
	Q_OBJECT
public:
	explicit Running(QObject *parent = nullptr);
	//void init(DynaxicelBase *control);//开启线程前要传入DynaxicelBase对象
	~Running();
public slots:
	void ReadPosInRunning(int *goalpos, int thres_);
	void GetPresentPos(uint16_t *pre_pos_);
	void MaxonIsDone();
signals:
	void GetPresentSingnal();
	void PresentPos(uint16_t *);
	void Done();
private:
	bool maxon_done = false;
	int thres;
	uint16_t pre_pos[4];
};
#endif
