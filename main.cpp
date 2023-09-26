#include <stdio.h>
#include <stdlib.h>

#include "stack.h"
#include "log_funcs.h"
#include "types.h"
#include "hash.h"

int main(const int argc, const char* argv[])
{
    OpenLogFile(argv[0]);

    Stack_t stk            = {};
    struct ErrorInfo error = {};

    struct ErrorInfo* error_ptr = &error;

    error.code = (ERRORS) StackCtor(&stk, 6);
    EXIT_ERROR(error_ptr);
}
