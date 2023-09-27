/*! \file
* \brief Contains all info about typed
*/

#include <math.h>

/// stack element type
typedef double elem_t;
/// stack type
typedef struct Stack Stack_t;

/// hash type
typedef unsigned int hash_t;
/// hash function type
typedef hash_t (*hash_f) (const void* obj, size_t size);
/// canary type
typedef long long unsigned int canary_t;

typedef int (*dump_f)(FILE*, const Stack_t*, const char*, const char*, const int);

#define PRINT_ELEM_T "%lf"
