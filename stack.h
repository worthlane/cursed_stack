#ifndef __STACK_H_
#define __STACK_H_

#include <stdio.h>

#include "errors.h"
#include "types.h"

/*! \file
* \brief Contains hash functions
*/

#ifndef CANARY_PROTECT
/************************************************************//**
 * @brief Canary protection (if canary gets changed, program returns error)
 *
 * 1 for ON
 * 0 for OFF
 ************************************************************/
#define CANARY_PROTECT 1

#endif

#ifndef HASH_PROTECT
/************************************************************//**
 * @brief Hash protection (if hash is not equal to hash, that we expected, program returns error)
 *
 * 1 for ON
 * 0 for OFF
 ************************************************************/
#define HASH_PROTECT 1

#endif

#if CANARY_PROTECT
#define ON_CANARY(...) __VA_ARGS__
#define OFF_CANARY(...) ;

#else
#define ON_CANARY(...) ;
#define OFF_CANARY(...) __VA_ARGS__
#endif

#if HASH_PROTECT
#define ON_HASH(...) __VA_ARGS__

#else
#define ON_HASH(...) ;
#endif

static const size_t MIN_CAPACITY = 1;           /// minimal stack capacity

static const canary_t canary_val = 0xDEADDEAD;  /// canary value

/// @brief Stack structure
struct Stack
{
    ON_CANARY
    (
        canary_t stack_prefix;
    )

    elem_t* data;
    size_t size;
    size_t capacity;
    size_t reserved;

    ON_HASH
    (
        hash_f hash_func;
        hash_t data_hash;
        hash_t stack_hash;
    )

    ON_CANARY
    (
        canary_t stack_postfix;
    )
};

/************************************************************//**
 * @brief Creates stack
 *
 * @param[in] stk stack pointer
 * @param[in] capacity stack capacity
 * @return int error code
 *************************************************************/
int StackCtor(Stack_t* stk, size_t capacity);

/************************************************************//**
 * @brief Destroys stack
 *
 * @param[in] stk stack pointer
 * @return int error code
 ************************************************************/
int StackDtor(Stack_t* stk);

/************************************************************//**
 * @brief Pushes element in stack
 *
 * @param[in] stk stack pointer
 * @param[in] value element
 * @return int error code
 ************************************************************/
int StackPush(Stack_t* stk, elem_t value);

/************************************************************//**
 * @brief Changes stack capacity
 *
 * @param[in] stk stack pointer
 * @param[in] new_capacity new stack capacity
 * @return int error code
 *************************************************************/
int StackRealloc(Stack_t* stk, size_t new_capacity);

/************************************************************//**
 * @brief Pops element from stack
 *
 * @param[in] stk stack pointer
 * @param[out] ret_value popped element
 * @return int error code
 ************************************************************/
int StackPop(Stack_t* stk, elem_t* ret_value);

/************************************************************//**
 * @brief Prints info about stack in output stream
 *
 * @param[in] fp output stream
 * @param[in] stk stack pointer
 * @param[in] func function, where print called
 * @param[in] file file, where print called
 * @param[in] line line, where print caled
 * @return int error code
 ************************************************************/
int StackDump(FILE* fp, const Stack_t* stk, const char* func, const char* file, const int line);

/************************************************************//**
 * @brief Verifies stack
 *
 * @param[in] stk stack pointer
 * @return int stack condition code
 ************************************************************/
int StackOk(Stack_t* stk);

/************************************************************//**
 * @brief Counts data hash
 *
 * @param[in] stk stack pointer
 * @return hash_t data hash
 ***********************************************************/
hash_t ReInitDataHash(const Stack_t* stk);

/************************************************************//**
 * @brief Counts stack hash
 *
 * @param[in] stk stack pointer
 * @return hash_t stack hash
 ************************************************************/
hash_t ReInitStackHash(const Stack_t* stk);

#endif
