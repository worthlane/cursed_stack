#include <stdio.h>

#include "errors.h"
#include "mylib/colorlib.h"
#include "log_funcs.h"
#include "stack.h"
#include "types.h"

int PrintError(FILE* fp, struct ErrorInfo* error, const char* func, const char* file, const int line)
{
    LOG_START_MOD(func, file, line);

    switch (error->code)
    {
        case (ERRORS::NONE):
            LOG_END();
            return (int) error->code;

        case (ERRORS::OPEN_FILE):
            fprintf(fp, "OPEN FILE ERROR\n"
                        "FAILED TO OPEN FILE \"%s\"\n", error->data);
            LOG_END();
            return (int) error->code;

        case (ERRORS::READ_FILE):
            fprintf(fp, "READ FILE ERROR\n"
                        "FAILED TO READ INFO FROM FILE \"%s\"\n", error->data);
            LOG_END();
            return (int) error->code;

        case (ERRORS::ALLOCATE_MEMORY):
            fprintf(fp, "MEMORY ALLOCATE ERROR\n"
                        "FAILED TO ALLOCATE MEMORY IN \"%s\"\n", error->data);
            LOG_END();
            return (int) error->code;

        case (ERRORS::PRINT_DATA):
            fprintf(fp, "DATA PRINT ERROR\n"
                        "FAILED TO PRINT DATA IN \"%s\"\n", error->data);
            LOG_END();
            return (int) error->code;

        case (ERRORS::INVALID_STACK):
            fprintf(fp, "INVALID STACK ERROR\n", error->data);
            LOG_END();
            return (int) error->code;

        default:
            fprintf(fp, "UNKNOWN ERROR\n");
            LOG_END();
            return (int) ERRORS::UNKNOWN;
    }
}

//-----------------------------------------------------------------------------------------------------

void PrintStackCondition(FILE* fp, int error, const Stack_t* stk)
{
    LOG_PRINT(">>>>>>>>>>STACK CONDITIONS<<<<<<<<<\n");

    if ((error & INVALID_CAPACITY) != 0)
        fprintf(fp, "INVALID STACK CAPACITY\n"
                    "SIZE:     %zu\n"
                    "CAPACITY: %zu\n",
                    stk->size, stk->capacity);

    if ((error & INVALID_SIZE) != 0)
        fprintf(fp, "INVALID STACK SIZE\n"
                    "SIZE:     %zu\n",
                    stk->size);

    if ((error & INVALID_DATA) != 0)
        fprintf(fp, "INVALID STACK DATA\n"
                    "DATA:     [%p]\n",
                    stk->data);

    #if CANARY_PROTECT
    canary_t* prefix_canary  = (canary_t*)((char*) stk->data - sizeof(canary_t));
    canary_t* postfix_canary = (canary_t*)((char*) stk->data + stk->capacity * sizeof(elem_t));

    if ((error & DATA_CANARY_TRIGGER) != 0)
        fprintf(fp, "DATA CANARY TRIGGERED\n"
                    "LEFT CANARY:     %zu\n"
                    "RIGHT CANARY:    %zu\n",
                    *prefix_canary, *postfix_canary);

    if ((error & STACK_CANARY_TRIGGER) != 0)
        fprintf(fp, "STACK CANARY TRIGGERED\n"
                    "LEFT CANARY:     %zu\n"
                    "RIGHT CANARY:    %zu\n",
                    stk->stack_prefix, stk->stack_postfix);
    #endif

    #if HASH_PROTECT

    if ((error & INVALID_HASH_FUNC) != 0)
        fprintf(fp, "INVALID HASH FUNCTION\n"
                    "FUNC:     [%p]\n",
                    stk->hash_func);

    if ((error & INCORRECT_DATA_HASH) != 0)
        fprintf(fp, "INCORRECT DATA HASH\n"
                    "EXPECTED:     %u\n"
                    "CURRENT:      %u\n",
                    stk->data_hash,
                    ReInitDataHash(stk));

    if ((error & INCORRECT_STACK_HASH) != 0)
        fprintf(fp, "INCORRECT DATA HASH\n"
                    "EXPECTED:     %u\n"
                    "CURRENT:      %u\n",
                    stk->stack_hash,
                    ReInitStackHash(stk));
    #endif

    LOG_PRINT(">>>>>>>>STACK CONDITIONS END<<<<<<<\n");
}
