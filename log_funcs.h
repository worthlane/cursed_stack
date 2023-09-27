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
 * @brief Prints separator in output stream
 *
 * @param[in] fp output stream
 ***********************************************************/
void PrintSeparator(FILE* fp);

/************************************************************//**
 * @brief Prints elements(data) from stack
 *
 * @param[in] fp output stream
 * @param[in] stk stack pointer
 * @return int error code
 ************************************************************/
int PrintStackData(FILE* fp, const Stack_t* stk);

#define LOG_PRINT(...)      fprintf(__LOG_STREAM__, __VA_ARGS__)

#define LOG_START()         fprintf(__LOG_STREAM__,                                         \
                                    "--------------------------------------------------\n"  \
                                    "RUNNING FUNCTION %s FROM FILE \"%s\"(%d)\n",           \
                                    __func__, __FILE__, __LINE__)

#define LOG_END()           fprintf(__LOG_STREAM__,                                         \
                                    "END TIME: %s\n"                                        \
                                    "--------------------------------------------------\n", \
                                    __TIME__)

#define STACK_DUMP(stk)                     StackDump(__LOG_STREAM__, stk, __func__, __FILE__, __LINE__)

#define LOG_SEPARATOR()                     PrintSeparator(__LOG_STREAM__)

#define LOG_START_MOD(func, file, line)     fprintf(__LOG_STREAM__,                                         \
                                                    "--------------------LOG CALLED--------------------\n"  \
                                                    "RUNNING FUNCTION %s FROM FILE \"%s\"(%d)\n",           \
                                                    func, file, line)

#define EXIT_IF_ERROR(error)                if (error->code != ERRORS::NONE)                       \
                                            {                                                      \
                                                return PrintError(__LOG_STREAM__, error, __func__, \
                                                                  __FILE__, __LINE__);             \
                                            }



#endif
