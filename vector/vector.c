#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "vector.h"

#define MAKE_VECTOR(TYPE) \
DEF_VECTOR(TYPE) \
TYPE ## _vector_result new_ ## TYPE ## _vector() \
{ \
    TYPE ## _vector_result result; \
    result.is_ok = true; \
    TYPE * buf = malloc(sizeof(TYPE) * DEFAULT_CAPACITY); \
    if (buf == NULL) \
    { \
        result.is_ok = false; \
    } \
    TYPE ## _vector v; \
    v.length = 0; \
    v.capacity = DEFAULT_CAPACITY; \
    v.data = buf; \
    result.inner.vector = v; \
    return result; \
} \
\
int push_ ## TYPE ## _vector(TYPE ##_vector *v, TYPE element) \
{ \
    if (v->length + 1 >= v->capacity) \
    { \
        int result = expand_vector((vector*)v, DEFAULT_CAPACITY); \
        if (!result) \
        { \
            return 0; \
        } \
    } \
    v->data[v->length] = element; \
    v->length++; \
    return 1; \
} \
TYPE pop_ ## TYPE ## _vector(TYPE ##_vector *v) \
{ \
    if (v->length < 1) \
    { \
        exit(1); \
    } \
    v->length--; \
    return v->data[v->length]; \
}

void free_vector(vector *v)
{
    free(v->data);
    v->data = NULL;
}

int expand_vector(vector *v, size_t size)
{
    v->capacity += size;
    void * realloc_result = realloc(v->data, v->capacity);
    if (realloc_result == NULL)
    {
        return 0;
    }
    return 1;
}

int shrink_vector(vector *v)
{
    if (v->capacity != v-> length)
    {
        void * realloc_result = realloc(v->data, v->length);
        if (realloc_result == NULL)
        {
            return 0;
        }
        v->capacity = v->length;
    }
    return 1;
}

MAKE_VECTOR(int)

int main(int argc, char *argv[])
{
    // test
    int_vector_result result = new_int_vector();
    int_vector v;
    assert(result.is_ok);
    v = result.inner.vector;
    assert(expand_vector((vector*)&v, 16));
    assert(v.length == 0);
    push_int_vector(&v, 1);
    push_int_vector(&v, 2);
    push_int_vector(&v, 3);
    assert(v.length == 3);
    int r3 = pop_int_vector(&v);
    int r2 = pop_int_vector(&v);
    int r1 = pop_int_vector(&v);
    assert(r3 == 3);
    assert(r2 == 2);
    assert(r1 == 1);
    free_vector((vector*)&v);
}
