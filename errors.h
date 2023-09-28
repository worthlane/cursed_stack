#ifndef __ERRORS_H
#define __ERRORS_H

/*! \file
* \brief Contains enumerated errors
*/

#include "types.h"

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
    /// popping element from empty stack
    EMPTY_STACK,

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

/// @brief list of stack conditions
enum StackCondition
{
    /// everything is fine
    OK                   = 0,

    /// stack has invalid capacity (<= 0)
    INVALID_CAPACITY     = 1 << 1,
    /// stack has invalid size (> capacity or < 0)
    INVALID_SIZE         = 1 << 2,
    /// stack contains invalid data
    INVALID_DATA         = 1 << 3,

    /// data canary triggered (canary protection mode)
    DATA_CANARY_TRIGGER  = 1 << 4,
    /// stack canary triggered (canary protection mode)
    STACK_CANARY_TRIGGER = 1 << 5,

    /// invalid hash function (hash protect mode)
    INVALID_HASH_FUNC    = 1 << 6,
    /// not expected data hash (hash protect mode)
    INCORRECT_DATA_HASH  = 1 << 7,
    /// not expected stack hash (hash protect mode)
    INCORRECT_STACK_HASH = 1 << 8
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
