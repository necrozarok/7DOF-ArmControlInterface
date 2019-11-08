#include "log.h"
logger::logger()
{
}

std::ostringstream& logger::Add(logLevel level)
{
	os << " " << getTime();
	os << " " << getLevel(level);
	os << ": ";
	return os;
}

std::string logger::getLevel(logLevel level)
{
	static const char* const levelValue[] = { "ERROR", "WARNING", "INFO", "DEBUG" };
	return levelValue[level];
}

void logger::output2Console()
{
	cout << os.str();
}

void logger::output2File()
{
	ofstream of("MaxonTestLog.txt", ofstream::app);
	of << os.str();
}

#if defined(_WIN32) || defined(_WIN64)

#include <windows.h>

std::string logger::getTime()
{
	const int MAX_LEN = 200;
	char buffer[MAX_LEN];
	//if (GetTimeFormatA(LOCALE_USER_DEFAULT, 0, 0,
	//	"HH':'mm':'ss", buffer, MAX_LEN) == 0)
	//	return "Error in getTime()";

	std::time_t rawtime = std::time(0);//get current time
	struct tm * timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, sizeof(buffer), "%d-%m-%Y %I:%M:%S", timeinfo);
	std::string str(buffer);

	char result[100] = { 0 };
	static DWORD first = GetTickCount();
	std::sprintf(result, "%s.%03ld", buffer, (long)(GetTickCount() - first) % 1000);
	return result;
}

#else

#include <sys/time.h>

std::string logger::getTime()
{
	char buffer[11];
	time_t t;
	time(&t);
	tm r = { 0 };
	strftime(buffer, sizeof(buffer), "%X", localtime_r(&t, &r));
	struct timeval tv;
	gettimeofday(&tv, 0);
	char result[100] = { 0 };
	std::sprintf(result, "%s.%03ld", buffer, (long)tv.tv_usec / 1000);
	return result;
}

#endif //WIN32


logger::~logger()
{
}