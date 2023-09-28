#include <stdio.h>
#include <assert.h>

#include "errors.h"
#include "log_funcs.h"
#include "types.h"

int PrintError(FILE* fp, const void* err, const char* func, const char* file, const int line)
{
    assert(err);

    LOG_START_MOD(func, file, line);

    struct ErrorInfo* error = (struct ErrorInfo*) err;

    switch (error->code)
    {
        case (ERRORS::NONE):
            LOG_END();
            return (int) error->code;

        case (ERRORS::OPEN_FILE):
            fprintf(fp, "OPEN FILE ERROR\n"
                        "FAILED TO OPEN FILE \"%s\"\n", (char*) error->data);
            LOG_END();
            return (int) error->code;

        case (ERRORS::READ_FILE):
            fprintf(fp, "READ FILE ERROR\n"
                        "FAILED TO READ INFO FROM FILE \"%s\"\n", (char*) error->data);
            LOG_END();
            return (int) error->code;

        case (ERRORS::ALLOCATE_MEMORY):
            fprintf(fp, "MEMORY ALLOCATE ERROR\n"
                        "FAILED TO ALLOCATE MEMORY IN \"%s\"\n", (char*) error->data);
            LOG_END();
            return (int) error->code;

        case (ERRORS::PRINT_DATA):
            fprintf(fp, "DATA PRINT ERROR\n"
                        "FAILED TO PRINT DATA IN \"%s\"\n", (char*) error->data);
            LOG_END();
            return (int) error->code;

        case (ERRORS::INVALID_STACK):
            fprintf(fp, "INVALID STACK ERROR\n");
            LOG_END();
            return (int) error->code;

        case (ERRORS::EMPTY_STACK):
            fprintf(fp, "CAN'T POP ELEMENT FROM EMPTY STACK\n");
            LOG_END();
            return (int) error->code;

        case (ERRORS::UNKNOWN):
        default:
            fprintf(fp, "UNKNOWN ERROR\n");
            LOG_END();
            return (int) ERRORS::UNKNOWN;
    }
}
