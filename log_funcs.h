#ifndef __LOG_FUNCS_H_
#define __LOG_FUNCS_H_

#include <stdio.h>

#include "types.h"

/*! \file
* \brief Contains log functions
*/

extern FILE* __LOG_STREAM__;

static const size_t MAX_FILE_NAME_LEN = 100;

/************************************************************//**
 * @brief Opens log file, also close it when program shuts down
 *
 * @param[in] FILE_NAME name of log file
 ************************************************************/
void OpenLogFile(const char* FILE_NAME);

/************************************************************//**
 * @brief Closes log file
 ************************************************************/
void CloseLogFile();

/************************************************************//**
 * @brief Prints elements(data) from stack
 *
 * @param[in] fp output stream
 * @param[in] stk stack pointer
 * @return int error code
 ************************************************************/
int PrintStackData(FILE* fp, const Stack_t* stk);

int LogDump(dump_f dump_func, const void* stk, const char* func, const char* file, const int line);

int PrintLog (const char *format, ...);

#ifdef LOG_START
#undef LOG_START

#endif
#define LOG_START()         PrintLog("--------------------------------------------------\n"  \
                                     "RUNNING FUNCTION %s FROM FILE \"%s\"(%d)\n",           \
                                     __func__, __FILE__, __LINE__)

#ifdef LOG_END
#undef LOG_END

#endif
#define LOG_END()           PrintLog("END TIME: %s\n"                                        \
                                     "--------------------------------------------------\n", \
                                     __TIME__)

#ifdef LOG_SEPARATOR
#undef LOG_SEPARATOR

#endif
#define LOG_SEPARATOR()     PrintLog("\n........................................\n\n");

#ifdef LOG_START_MOD
#undef LOG_START_MOD

#endif
#define LOG_START_MOD(func, file, line)     PrintLog(                                                       \
                                                    "--------------------LOG CALLED--------------------\n"  \
                                                    "RUNNING FUNCTION %s FROM FILE \"%s\"(%d)\n",           \
                                                    func, file, line)

#ifdef EXIT_IF_ERROR
#undef EXIT_IF_ERROR

#endif
#define EXIT_IF_ERROR(error)                if (error->code != ERRORS::NONE)                        \
                                            {                                                       \
                                                return LogDump(PrintError, error, __func__,         \
                                                                  __FILE__, __LINE__);              \
                                            }



#endif
