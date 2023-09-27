#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <strings.h>

#include "log_funcs.h"
#include "errors.h"
#include "stack.h"
#include "types.h"

FILE* __LOG_STREAM__ = stderr;

static const char* EXTENSION = ".log";

void OpenLogFile(const char* FILE_NAME)
{
    char* file_name = strdup(FILE_NAME);

    __LOG_STREAM__ = fopen(strncat(file_name, EXTENSION, MAX_FILE_NAME_LEN), "a");

    if (__LOG_STREAM__ == nullptr)
        __LOG_STREAM__ = stderr;

    time_t now = 0;
    time(&now);

    fprintf(__LOG_STREAM__, "\n*********************************************************************\n"
                              "=========================== PROGRAM START ===========================\n"
                              "*********************************************************************\n"
                              "RUNNED AT %s\n", ctime(&now));

    #if CANARY_PROTECT
        fprintf(__LOG_STREAM__, "[CANARY PROTECT ON]\n");
    #endif

    #if HASH_PROTECT
        fprintf(__LOG_STREAM__, "[HASH PROTECT ON]\n");
    #endif

    fputc('\n', __LOG_STREAM__);

    atexit(CloseLogFile);
}

//-----------------------------------------------------------------------------------------------------

void PrintSeparator(FILE* fp)
{
    fprintf(fp, "\n........................................\n\n");
}

//-----------------------------------------------------------------------------------------------------

void CloseLogFile()
{
    fprintf(__LOG_STREAM__, "*********************************************************************\n"
                            "============================ PROGRAM END ============================\n"
                            "*********************************************************************\n");
    fclose(__LOG_STREAM__);
}

//-----------------------------------------------------------------------------------------------------

int PrintStackData(FILE* fp, const Stack_t* stk)
{
    for (size_t i = 0; i < stk->size; i++)
    {
        fprintf(fp, "*[%zu] > " PRINT_ELEM_T "\n", i, stk->data[i]);
    }

    fprintf(fp, "clear elements\n");

    for (size_t i = stk->size; i < stk->capacity; i++)
    {
        fprintf(fp, "*[%zu] > " PRINT_ELEM_T "\n", i, stk->data[i]);
    }

    return (int) ERRORS::NONE;
}

//-----------------------------------------------------------------------------------------------------

int LogDump(dump_f dump_func, const Stack_t* stk, const char* func, const char* file, const int line)
{
    assert(dump_func);
    assert(stk);

    return dump_func(__LOG_STREAM__, stk, func, file, line);
}

