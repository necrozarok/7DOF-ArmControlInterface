#pragma once
#ifndef __LOGGER_H__
#define __LOGGER_H__

#define _CRT_SECURE_NO_WARNINGS	//for using sprintf

#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <ctime>

#if defined(_WIN32) || defined(_WIN64)
// Win Socket Header File(s)
#include <Windows.h>
#include <process.h>
#else
// POSIX Socket Header File(s)
#include <errno.h>
#include <pthread.h>
#endif


using namespace std;

enum logLevel
{
	logERROR = 0,
	logWARNING = 1,
	logINFO = 2,
	logDEBUG = 3
};

class logger
{
public:

	logger();
	~logger();

	ostringstream& Add(logLevel level = logINFO);
	static string getTime();
	static string getLevel(logLevel level);

	void output2Console();
	void output2File();
	//logger(const logger&);

private:
	ostringstream os;


};
#endif
