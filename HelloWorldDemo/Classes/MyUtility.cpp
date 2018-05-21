#include "MyUtility.h"

string MyUtility::getCurrentTime()
{
	time_t t;
	time(&t);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y-%m-%d %X", localtime((&t)));
	string timeStr = tmp;
	return timeStr;
}