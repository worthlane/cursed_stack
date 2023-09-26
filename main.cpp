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

    if ((error.code = (ERRORS) StackCtor(&stk, 6)) != ERRORS::NONE)
        LOG_ERROR(&error);

    StackPush(&stk, 1);
    StackPush(&stk, 1);
    StackPush(&stk, 1);
    StackPush(&stk, 1);
    StackPush(&stk, 1);
    StackPush(&stk, 1);
    StackPush(&stk, 1);
    StackPush(&stk, 1);

    elem_t a = 1;

    STACK_DUMP(&stk);

    StackPop(&stk, &a);
    StackPop(&stk, &a);
    StackPop(&stk, &a);
    StackPop(&stk, &a);
    StackPop(&stk, &a);
    StackPop(&stk, &a);
    StackPop(&stk, &a);

    STACK_DUMP(&stk);

    StackPush(&stk, 2);
    StackPush(&stk, 2);
    StackPush(&stk, 2);
    StackPush(&stk, 2);
    StackPush(&stk, 2);
    StackPush(&stk, 2);
    StackPush(&stk, 2);
    StackPush(&stk, 2);

    STACK_DUMP(&stk);

    stk.data[3] = 8;

    STACK_DUMP(&stk);
}
