#include <stdio.h>
#include <stdlib.h>

#include "stack.h"
#include "log_funcs.h"
#include "types.h"
#include "hash.h"

int main(const int argc, const char* argv[])
{
    if (argc == 1)
        OpenLogFile(argv[0]);
    else
        OpenLogFile(argv[1]);

    Stack_t stk            = {};
    struct ErrorInfo error = {};

    struct ErrorInfo* error_ptr = &error;

    error.code = (ERRORS) StackCtor(&stk, 6);
    EXIT_IF_ERROR(error_ptr);

    StackPush(&stk, 7);
    StackPush(&stk, 7);
    StackPush(&stk, 7);
    StackPush(&stk, 7);
    StackPush(&stk, 7);
    StackPush(&stk, 7);
    StackPush(&stk, 7);
    StackPush(&stk, 7);

    elem_t a = 0;

    StackPop(&stk, &a);
    StackPop(&stk, &a);
    StackPop(&stk, &a);
    StackPop(&stk, &a);
    error.code = (ERRORS) StackPop(&stk, &a);
    stk.data[1] = 228;
    error.code = (ERRORS) StackPop(&stk, &a);
    EXIT_IF_ERROR(error_ptr);

    STACK_DUMP(&stk);
}
