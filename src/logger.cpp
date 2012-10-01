#include "perflib.h"

tLogMessage loglist[LOGLISTLEN];

bool LoggerRun = false;
volatile LONG LogNextIn = -1;
volatile LONG LogEntrysLeft = 0;
HANDLE hLogThread;

void add2log(const char *text)
{
	unsigned int logindex;
	logindex = InterlockedIncrement(&LogNextIn) % (LOGLISTLEN); 
	struct timeb tp;
	ftime(&tp);
	char timeString[80];
	strftime(timeString, sizeof(timeString), "%H:%M:%S", localtime(&tp.time));
	sprintf_s( loglist[logindex].Entry, LOGENTRYSIZE, "%s %s\n", timeString, text);
	loglist[logindex].Valid = true;

	InterlockedIncrement(&LogEntrysLeft);
}

unsigned __stdcall LogThreaded(void * dummy)
{
	unsigned int lognextout = 0;
	FILE *fph;
	while (LoggerRun == true)
	{
		while ( loglist[lognextout].Valid == false)	Sleep(100);

		fph = fopen("blissperflib.log", "a");
		fprintf_s( fph, "%s", loglist[lognextout].Entry);
		fclose(fph);

		loglist[lognextout].Valid = false;
		lognextout = ++lognextout % LOGLISTLEN;
		InterlockedDecrement(&LogEntrysLeft); 
	}
	return 0;
}


