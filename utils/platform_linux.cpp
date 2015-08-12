#ifdef __linux__
#include "sys/types.h"
#include "sys/sysinfo.h"
#include <stdio.h>
#include <cstdlib>
#include <string.h>

#include "platformDependent.h"

const char* eoln()
{
return "\n";
}

size_t eoln_len()
{
    return 1;
}

int parseLine(char* line){
        int i = strlen(line);
        while (*line < '0' || *line > '9') line++;
        line[i-3] = '\0';
        i = atoi(line);
        return i;
    }

long current_used_memory()
{
    FILE* file = fopen("/proc/self/status", "r");
    int result = -1;
    char line[128];

    while (fgets(line, 128, file) != NULL){
        if (strncmp(line, "VmData:", 7) == 0){
            result = parseLine(line);
            break;
        }
    }
    fclose(file);
    return result * 1024;

}

#endif
