#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

// Windows Header Files:
#include <windows.h>
#include <process.h>
#include <sys/timeb.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#define PCLISTLEN 256
#define TSLISTLEN 256
#define LOGLISTLEN 4096
#define LOGENTRYSIZE 4096
#define SMALSTRINGSIZE 64
#define FUNCTIONSIZE 2048
#define OUTPUTSIZE 4096

struct tTN
{
	char ThreadName[SMALSTRINGSIZE];
	double TotalDuration;
	unsigned int Count;
};

struct tPC
{
	LARGE_INTEGER PCS;
	unsigned int ThreadType;
};

struct tLogMessage
{
	char *Entry;
	bool Valid;
};

extern bool LoggerRun;
extern tLogMessage loglist[];
extern HANDLE hLogThread;
extern volatile LONG LogEntrysLeft;

void add2log(const char *text);
unsigned __stdcall LogThreaded(void * dummy);
