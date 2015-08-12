#ifdef _WIN32
#include <Windows.h>
#include <Psapi.h>
#include "platformDependent.h"

const char* eoln()
{
return "\r\n";
}

size_t eoln_len()
{
    return 2;
}

long current_used_memory()
{
	HANDLE pid = GetCurrentProcess();
	PROCESS_MEMORY_COUNTERS ppsmemCounters;
	if(!GetProcessMemoryInfo(pid, &ppsmemCounters,sizeof(ppsmemCounters)))
		return -1;
	return ppsmemCounters.WorkingSetSize;
}

#endif
