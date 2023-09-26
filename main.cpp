#include <stdio.h>
#include <stdlib.h>

#include "stack.h"
#include "log_funcs.h"
#include "types.h"
#include "hash.h"

int main(const int argc, const char* argv[])
{
    OpenLogFile(argv[0]);

    int* hack = (int*) calloc(1, sizeof(int));

    Stack_t stk            = {};
    struct ErrorInfo error = {};

    if ((error.code = (ERRORS) StackCtor(&stk, 2)) != ERRORS::NONE)
        LOG_ERROR(&error);

    /*LOG_PRINT("HACK ADRESS  [%zu]\n"
              "STACK ADRESS [%zu]\n"
              "DATA ADRESS  [%zu]\n", hack + 40, stk, stk.data);

    for (int i = 0; i < 40; i++)
    {
        *(hack + i) = 0;
    }*/

    // *((char*) stk.data - sizeof(canary_t)) = 's';

    // stk.data = 0;

    if ((error.code = (ERRORS) StackPush(&stk, 1)) != ERRORS::NONE)
        LOG_ERROR(&error);

    if ((error.code = (ERRORS) StackPush(&stk, 2)) != ERRORS::NONE)
        LOG_ERROR(&error);

    if ((error.code = (ERRORS) StackPush(&stk, 3)) != ERRORS::NONE)
        LOG_ERROR(&error);

    if ((error.code = (ERRORS) StackPush(&stk, 4)) != ERRORS::NONE)
        LOG_ERROR(&error);

    stk.data[2] = 228;

    if ((error.code = (ERRORS) StackPush(&stk, 5)) != ERRORS::NONE)
        LOG_ERROR(&error);

    elem_t a = 1;

    if ((error.code = (ERRORS) StackPop(&stk, &a)) != ERRORS::NONE)
        LOG_ERROR(&error);

    if ((error.code = (ERRORS) StackPop(&stk, &a)) != ERRORS::NONE)
        LOG_ERROR(&error);
    if ((error.code = (ERRORS) StackPop(&stk, &a)) != ERRORS::NONE)
        LOG_ERROR(&error);
    if ((error.code = (ERRORS) StackPop(&stk, &a)) != ERRORS::NONE)
        LOG_ERROR(&error);
    if ((error.code = (ERRORS) StackPop(&stk, &a)) != ERRORS::NONE)
        LOG_ERROR(&error);

    if ((error.code = (ERRORS) StackPush(&stk, 228)) != ERRORS::NONE)
        LOG_ERROR(&error);

    if ((error.code = (ERRORS) StackPop(&stk, &a)) != ERRORS::NONE)
        LOG_ERROR(&error);
}
