#ifndef RAYMI_CLIBS_VECTOR
#define RAYMI_CLIBS_VECTOR

#include <stddef.h>
#include <stdbool.h>

#define DEFAULT_CAPACITY 16

#define DEF_VECTOR(TYPE) \
typedef struct TYPE ## _vector { \
    size_t length; \
    size_t capacity; \
    TYPE * data; \
} TYPE ## _vector; \
typedef struct TYPE ## _vector_result { \
    bool is_ok; \
    union { \
        char error; \
        TYPE ## _vector vector; \
    } inner; \
} TYPE ## _vector_result;

typedef struct vector {
    size_t length;
    size_t capacity;
    void * data;
} vector;

#endif
