// dllmain.cpp : Defines the entry point for the DLL application.
#include "perflib.h"

LARGE_INTEGER PCF;
unsigned int ArmaThreadCount;
unsigned int PCNextFreeIndexPut, PCNextFreeIndexGet;	// indices for circular buffer
unsigned int PerfCounterIndices[PCLISTLEN];				// circular buffer

tTN ThreadStats[TSLISTLEN];								// array holding stats for threadtypes
tPC PerfCounterList[PCLISTLEN];							// array holding startvalues and threadtypes for performancecounter 

void Init()
{
	unsigned i;
	for(i=0; i<LOGLISTLEN; i++)
	{
		loglist[i].Entry = (char *) calloc(LOGENTRYSIZE, 1);
		loglist[i].Valid = false;
	};
	LoggerRun = true;
	hLogThread = (HANDLE)_beginthreadex(NULL, 0, LogThreaded, NULL, 0, NULL );

	for(i=0; i<PCLISTLEN; i++)
	{
		PerfCounterIndices[i] = i;
	};

	PCNextFreeIndexPut = 0;
	PCNextFreeIndexGet = 0;
	ArmaThreadCount = 0;
	QueryPerformanceFrequency(&PCF);

	char index[SMALSTRINGSIZE];
	for(i=0; i<TSLISTLEN; i++)
	{
		sprintf_s(index, SMALSTRINGSIZE, "%u", i);
		GetPrivateProfileString("profiler", index, "", ThreadStats[i].ThreadName, SMALSTRINGSIZE, ".\\blissperflib.ini");
		ThreadStats[i].TotalDuration = 0;
		ThreadStats[i].Count = 0;
//		add2log(ThreadNames[i].ThreadName);
	};
}

void Finit()
{
	unsigned i;

	while (LogEntrysLeft > 0) Sleep(100);
	for(i=0; i<LOGLISTLEN; i++)
	{
		if (loglist[i].Entry != NULL) free((void *)loglist[i].Entry);
	};
	CloseHandle( hLogThread );
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		Init();
	break;
	case DLL_THREAD_ATTACH:
	break;
	case DLL_THREAD_DETACH: 
	break;
	case DLL_PROCESS_DETACH:
		Finit();
	break;
	}
	return TRUE;
}

extern "C" __declspec(dllexport) void __stdcall RVExtension(char *output, int outputSize, const char *function)
{
// command format: I:ThreadType oder O:PCIndex
	LARGE_INTEGER PCE;
	double tn;
	unsigned int SavedIndex;
	unsigned int ThreadType;
	char tempS[FUNCTIONSIZE];

	if (*function =='I') 
	{
		QueryPerformanceCounter(&PerfCounterList[PerfCounterIndices[PCNextFreeIndexGet]].PCS);
		PerfCounterList[PerfCounterIndices[PCNextFreeIndexGet]].ThreadType = strtoul(&function[2], NULL, 10);
		sprintf_s( output, SMALSTRINGSIZE, "%u", PerfCounterIndices[PCNextFreeIndexGet]);
		PCNextFreeIndexGet = ++PCNextFreeIndexGet % PCLISTLEN;
		++ArmaThreadCount;
		return;
	}
	else if (*function =='O')
	{
		QueryPerformanceCounter(&PCE);
		SavedIndex = strtoul(&function[2], NULL, 10);
		tn = (double)(PCE.QuadPart - PerfCounterList[SavedIndex].PCS.QuadPart) / (double)PCF.QuadPart * 1000;
		ThreadType = PerfCounterList[SavedIndex].ThreadType;
		ThreadStats[ThreadType].TotalDuration += tn;
		ThreadStats[ThreadType].Count += 1;

		sprintf_s( tempS, FUNCTIONSIZE, "PT:%4u TC:%8u AD:%8.2fms %8.2fms : %s", ArmaThreadCount, ThreadStats[ThreadType].Count, ThreadStats[ThreadType].TotalDuration/ThreadStats[ThreadType].Count, tn, ThreadStats[ThreadType].ThreadName);
		add2log(tempS);

		PerfCounterIndices[PCNextFreeIndexPut] = SavedIndex;
		PCNextFreeIndexPut = ++PCNextFreeIndexPut % PCLISTLEN;
		--ArmaThreadCount;
		return;
	}
	else if (*function =='S') 
	{
		char *pstr_b, *pstr_e, *psep_b, *outp;
		int sep_len;

		pstr_b = (char*)function + 2;		// setting begin of string

		pstr_e = strstr(pstr_b, "~#~");		// finding magic code
		*pstr_e = 0;						// patch 0 to end of string

		psep_b = pstr_e + 3;				// setting begin of separator

		sep_len = strnlen_s(psep_b, 32);

		// construct output
		outp = output;
		*outp++ = '[';
		*outp++ = '"';
		while(pstr_e = strstr(pstr_b, psep_b))
		{
			while(pstr_b != pstr_e)
			{
				if (*pstr_b == '"') *outp++ = '"'; 
				*outp++ = *pstr_b++;
			};
			*outp++ = '"';
			*outp++ = ',';
			*outp++ = '"';
			pstr_b += sep_len;
		};

		while(*pstr_b != 0)
		{
			if (*pstr_b == '"') *outp++ = '"'; 
			*outp++ = *pstr_b++;
		};

		*outp++ = '"';
		*outp++ = ']';
		*outp = 0;

		return;
	};
}
