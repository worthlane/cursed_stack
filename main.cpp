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

    error.code = (ERRORS) StackCtor(&stk);
    EXIT_IF_ERROR(&error);

    STACK_DUMP(&stk);
    STACK_DUMP(&stk);

    error.code = (ERRORS) StackPush(&stk, 7);
    EXIT_IF_ERROR(&error);
    error.code = (ERRORS) StackPush(&stk, 7);
    EXIT_IF_ERROR(&error);
    error.code = (ERRORS) StackPush(&stk, 7);
    EXIT_IF_ERROR(&error);
    error.code = (ERRORS) StackPush(&stk, 7);
    EXIT_IF_ERROR(&error);
    error.code = (ERRORS) StackPush(&stk, 7);
    EXIT_IF_ERROR(&error);
    error.code = (ERRORS) StackPush(&stk, 7);
    EXIT_IF_ERROR(&error);
    error.code = (ERRORS) StackPush(&stk, 7);
    EXIT_IF_ERROR(&error);
    error.code = (ERRORS) StackPush(&stk, 7);
    EXIT_IF_ERROR(&error);

    elem_t a = 0;

    error.code = (ERRORS) StackPop(&stk, &a);
    EXIT_IF_ERROR(&error);
    error.code = (ERRORS) StackPop(&stk, &a);
    EXIT_IF_ERROR(&error);
    error.code = (ERRORS) StackPop(&stk, &a);
    EXIT_IF_ERROR(&error);
    error.code = (ERRORS) StackPop(&stk, &a);
    EXIT_IF_ERROR(&error);
    stk.data[1] = 9;
    error.code = (ERRORS) StackPop(&stk, &a);
    EXIT_IF_ERROR(&error);
    error.code = (ERRORS) StackPop(&stk, &a);
    EXIT_IF_ERROR(&error);


    STACK_DUMP(&stk);
}
