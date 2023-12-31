#ifndef __ERRORS_H
#define __ERRORS_H

/*! \file
* \brief Contains enumerated errors
*/

#include "types.h"

#ifdef EXIT_IF_ERROR
#undef EXIT_IF_ERROR

#endif
#define EXIT_IF_ERROR(error)                do                                                          \
                                            {                                                           \
                                                if ((error)->code != ERRORS::NONE)                      \
                                                {                                                       \
                                                    return LogDump(PrintError, error, __func__,         \
                                                                    __FILE__, __LINE__);                \
                                                }                                                       \
                                            } while(0)

/// @brief list of errors
enum class ERRORS
{
    /// not an error
    NONE = 0,

    /// error while opening file
    OPEN_FILE,
    /// error while reading file
    READ_FILE,

    /// error with memory allocating
    ALLOCATE_MEMORY,

    /// error while printing data
    PRINT_DATA,

    /// invalid stack error
    INVALID_STACK,

    /// unknown error
    UNKNOWN
};

/// @brief struct with all error info
struct ErrorInfo
{
    /// error code
    ERRORS code;
    /// error data
    void* data;
};

/************************************************************//**
 * @brief Prints error from error list
 *
 * @param[in] fp output stream
 * @param[in] error error
 * @param[in] func function where error happened
 * @param[in] file file where error happened
 * @param[in] line line where error happened
 * @return int error code
 ************************************************************/
int PrintError(FILE* fp, const void* error, const char* func, const char* file, const int line);

#endif
