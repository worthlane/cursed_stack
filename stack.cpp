#include <stdlib.h>
#include <assert.h>

#include "stack.h"
#include "errors.h"
#include "log_funcs.h"
#include "types.h"
#include "hash.h"

static void InitCanary(canary_t* prefix_canary, canary_t* postfix_canary);
static bool VerifyCanary(const canary_t* prefix_canary, const canary_t* postfix_canary);

static bool VerifyDataHash(const Stack_t* stk);
static bool VerifyStackHash(const Stack_t* stk);
static inline void ReInitAllHashes(Stack_t* stk);

static int Global_stack_error = 0;

int StackCtor(Stack_t* stk, size_t capacity)
{
    assert(stk);

    elem_t* data       = nullptr;
    size_t data_size   = capacity * sizeof(elem_t);

    ON_CANARY(data_size += 2 * sizeof(canary_t));

    data = (elem_t*) calloc(data_size, 1);

    if (data == nullptr)
        return (int) ERRORS::ALLOCATE_MEMORY;

    elem_t* first_elem = data;

    ON_CANARY
    (
        first_elem               = (elem_t*)((char*) data + sizeof(canary_t));
        canary_t* prefix_canary  = (canary_t*) data;
        canary_t* postfix_canary = (canary_t*)((char*) data + capacity * sizeof(elem_t) + sizeof(canary_t));

        InitCanary(prefix_canary, postfix_canary);

        InitCanary(&stk->stack_prefix, &stk->stack_postfix)
    );

    stk->data     = first_elem;
    stk->size     = 0;
    stk->capacity = capacity;

    ON_HASH
    (
        if (stk->hash_func == nullptr)
            stk->hash_func = MurmurHash
    );

    ReInitAllHashes(stk);

    CHECK_STACK(stk, Global_stack_error);

    return (int) ERRORS::NONE;
}

//-----------------------------------------------------------------------------------------------------

int StackDtor(Stack_t* stk)
{
    assert(stk);

    CHECK_STACK(stk, Global_stack_error);

    OFF_CANARY(elem_t* data = stk->data);

    ON_CANARY(elem_t* data = (elem_t*)((char*) stk->data - sizeof(canary_t)));

    free(data);

    stk->data     = nullptr;
    stk->size     = 0;
    stk->capacity = 0;

    ON_CANARY
    (
        stk->stack_prefix  = 0;
        stk->stack_postfix = 0
    );

    ON_HASH
    (
        stk->hash_func  = nullptr;
        stk->stack_hash = 0;
        stk->data_hash  = 0
    );

    return (int) ERRORS::NONE;
}

//-----------------------------------------------------------------------------------------------------

int StackPush(Stack_t* stk, elem_t value)
{
    assert(stk);
    assert(stk->data);

    CHECK_STACK(stk, Global_stack_error);

    if (stk->capacity == stk->size)
    {
        if (StackRealloc(stk, 2 * stk->capacity) != (int) ERRORS::NONE)
            return (int) ERRORS::ALLOCATE_MEMORY;
    }

    (stk->data)[(stk->size)++] = value;

    ReInitAllHashes(stk);

    CHECK_STACK(stk, Global_stack_error);

    return (int) ERRORS::NONE;
}

//-----------------------------------------------------------------------------------------------------

int StackRealloc(Stack_t* stk, size_t new_capacity)
{
    assert(stk);

    CHECK_STACK(stk, Global_stack_error);

    if (new_capacity == 0)
        new_capacity = MIN_CAPACITY;

    elem_t* data       = stk->data;
    elem_t* first_elem = data;
    size_t new_size    = new_capacity * sizeof(elem_t);

    ON_CANARY
    (
        new_size = new_size + 2 * sizeof(canary_t);
        data = (elem_t*)((char*) data - sizeof(canary_t))
    );

    elem_t* temp = (elem_t*) realloc(data, new_size);

    if (temp == nullptr)
    {
        StackDtor(stk);

        return (int) ERRORS::ALLOCATE_MEMORY;
    }
    else
        data = temp;

    ON_CANARY
    (
        first_elem               = (elem_t*)((char*) data + sizeof(canary_t));
        canary_t* postfix_canary = (canary_t*)((char*) first_elem + new_capacity * sizeof(elem_t));

        *(postfix_canary) = canary_val
    );

    stk->data     = first_elem;
    stk->capacity = new_capacity;

    ReInitAllHashes(stk);

    CHECK_STACK(stk, Global_stack_error);

    return (int) ERRORS::NONE;
}

//-----------------------------------------------------------------------------------------------------

int StackPop(Stack_t* stk, elem_t* ret_value)
{
    assert(stk);
    assert(stk->data);

    CHECK_STACK(stk, Global_stack_error);

    *(ret_value) = (stk->data)[--(stk->size)];
    (stk->data)[(stk->size)] = 0;

    ReInitAllHashes(stk);

    if (stk->size <= stk->capacity / 2)
    {
        int realloc_error  = StackRealloc(stk, stk->capacity / 2);
        if (realloc_error != (int) ERRORS::NONE)
            return realloc_error;
    }

    ReInitAllHashes(stk);

    CHECK_STACK(stk, Global_stack_error);

    return (int) ERRORS::NONE;
}

//-----------------------------------------------------------------------------------------------------

int StackOk(Stack_t* stk)
{
    assert(stk);

    int error = 0;

    ON_CANARY
    (
        canary_t* prefix_canary  = (canary_t*)((char*) stk->data - sizeof(canary_t));
        canary_t* postfix_canary = (canary_t*)((char*) stk->data + stk->capacity * sizeof(elem_t));

        if (!VerifyCanary(prefix_canary, postfix_canary))           error |= DATA_CANARY_TRIGGER;
        if (!VerifyCanary(&stk->stack_prefix, &stk->stack_postfix)) error |= DATA_CANARY_TRIGGER
    );

    if (stk->capacity <= 0)                                         error |= INVALID_CAPACITY;
    if (stk->size > stk->capacity || stk->size < 0)                 error |= INVALID_SIZE;
    if (stk->data == nullptr && stk->capacity != 0)                 error |= INVALID_DATA;

    ON_HASH
    (
        if (!stk->hash_func)                                        error |= INVALID_HASH_FUNC;

        if (!VerifyDataHash(stk))                                   error |= INCORRECT_DATA_HASH;
        if (!VerifyStackHash(stk))                                  error |= INCORRECT_STACK_HASH
    );

    return error;
}

//-----------------------------------------------------------------------------------------------------

static void InitCanary(canary_t* prefix_canary, canary_t* postfix_canary)
{
    assert(prefix_canary);
    assert(postfix_canary);
    assert(prefix_canary != postfix_canary);

    *prefix_canary  = canary_val;
    *postfix_canary = canary_val;
}

//-----------------------------------------------------------------------------------------------------

static bool VerifyCanary(const canary_t* prefix_canary, const canary_t* postfix_canary)
{
    assert(prefix_canary);
    assert(postfix_canary);
    assert(prefix_canary != postfix_canary);

    if (*prefix_canary  != canary_val)
        return false;

    if (*postfix_canary != canary_val)
        return false;

    return true;
}

//-----------------------------------------------------------------------------------------------------

static bool VerifyStackHash(const Stack_t* stk)
{
    assert(stk);

    ON_HASH
    (
        hash_t current_hash = stk->stack_hash;

        if (current_hash != ReInitStackHash(stk))
            return false;

        return true
    );

    return false;
}

//-----------------------------------------------------------------------------------------------------

static bool VerifyDataHash(const Stack_t* stk)
{
    assert(stk);

    ON_HASH
    (
        hash_t current_hash = stk->data_hash;

        if (current_hash != ReInitDataHash(stk))
            return false;

        return true
    );

    return false;
}

//-----------------------------------------------------------------------------------------------------

hash_t ReInitDataHash(const Stack_t* stk)
{
    assert(stk);

    hash_t new_hash = 0;

    ON_HASH
    (
        elem_t* data = stk->data;
        size_t data_size = stk->capacity * sizeof(elem_t);

        ON_CANARY
        (
            data      = (elem_t*)((char*) data - sizeof(canary_t));
            data_size = data_size + 2 * sizeof(canary_t)
        );

        new_hash = stk->hash_func(data, data_size)
    );

    return new_hash;
}

//-----------------------------------------------------------------------------------------------------

hash_t ReInitStackHash(const Stack_t* stk)
{
    assert(stk);

    hash_t new_hash = 0;

    ON_HASH
    (
        hash_t stack_hash = stk->stack_hash;
        ((Stack_t*) stk)->stack_hash = 0;

        new_hash = stk->hash_func(stk, sizeof(Stack_t));

        ((Stack_t*) stk)->stack_hash = stack_hash
    );

    return new_hash;;
}

//-----------------------------------------------------------------------------------------------------

int StackDump(FILE* fp, const Stack_t* stk, const char* func, const char* file, const int line)
{
    assert(stk);
    assert(func);
    assert(file);

    LOG_START_MOD(func, file, line);

    fprintf(fp, "Stack                > [%p]\n"
                "size                 > %zu\n"
                "capacity             > %zu\n"
                "data place           > [%p]\n",
                stk, stk->size, stk->capacity, stk->data);

    ON_CANARY
    (
        fprintf(fp, "STACK PREFIX CANARY  > %X\n"
                    "STACK POSTFIX CANARY > %X\n",
                    stk->stack_prefix, stk->stack_postfix)
    );

    ON_HASH
    (
        fprintf(fp, "HASH FUNCTION        > [%p]\n"
                    "::::::EXPECTED HASH::::::\n"
                    "STACK HASH           > %u\n"
                    "DATA HASH            > %u\n"
                    "::::::CURRENT HASH::::::\n"
                    "STACK CURRENT        > %u\n"
                    "DATA CURRENT         > %u\n",
                    stk->hash_func, stk->stack_hash, stk->data_hash,
                    ReInitStackHash(stk), ReInitDataHash(stk))
    );

    fprintf(fp, "ELEMENTS: \n\n");

    PrintStackData(fp, stk);

    ON_CANARY
    (
        canary_t* prefix_canary  = (canary_t*)((char*) stk->data - sizeof(canary_t));
        canary_t* postfix_canary = (canary_t*)((char*) stk->data + stk->capacity * sizeof(elem_t));

        fprintf(fp, "PREFIX DATA CANARY  > %X\n"
                    "POSTFIX DATA CANARY > %X\n", *prefix_canary, *postfix_canary)
    );

    LOG_END();

    return (int) ERRORS::NONE;
}

//-----------------------------------------------------------------------------------------------------

static inline void ReInitAllHashes(Stack_t* stk)
{
    ON_HASH
    (
        stk->data_hash  = ReInitDataHash(stk);
        stk->stack_hash = ReInitStackHash(stk)
    );
}
