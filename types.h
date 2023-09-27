/*! \file
* \brief Contains all info about typed
*/

/// stack element type
typedef long long int elem_t;
/// stack type
typedef struct Stack Stack_t;

/// hash type
typedef unsigned int hash_t;
/// hash function type
typedef hash_t (*hash_f) (const void* obj, size_t size);
/// canary type
typedef long long unsigned int canary_t;

#define PRINT_ELEM_T "%lld"
