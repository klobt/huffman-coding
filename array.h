#ifndef KLOBT_HUFFMAN_CODING__ARRAY_
#define KLOBT_HUFFMAN_CODING__ARRAY_

#include <stddef.h>
#include <stdlib.h>
#include "utilities.h"

#define DEFINE_ARRAY(name, type)                                        \
    typedef struct {                                                    \
        type *elements;                                                 \
        size_t capacity;                                                \
        size_t size;                                                    \
    } name##_array_t;                                                   \
                                                                        \
    name##_array_t *name##_array_create();                              \
    void name##_array_free(name##_array_t *array);                      \
    void name##_array_add(name##_array_t *array, type element);         \
    type name##_array_pop(name##_array_t *array);

#define IMPLEMENT_ARRAY(name, type)                                     \
    name##_array_t *name##_array_create() {                             \
        name##_array_t *array;                                          \
                                                                        \
        array = malloc(sizeof(type));                                   \
        array->elements = malloc(sizeof(type));                         \
        array->capacity = 1;                                            \
        array->size = 0;                                                \
                                                                        \
        return array;                                                   \
    }                                                                   \
                                                                        \
    void name##_array_free(name##_array_t *array) {                     \
        free(array->elements);                                          \
        free(array);                                                    \
    }                                                                   \
                                                                        \
    void name##_array_add(name##_array_t *array, type element) {        \
        if (array->size == array->capacity) {                           \
            array->capacity *= 2;                                       \
            array->elements = realloc(                                  \
                array->elements,                                        \
                sizeof(type) * array->capacity                          \
            );                                                          \
        }                                                               \
                                                                        \
        array->elements[array->size++] = element;                       \
    }                                                                   \
                                                                        \
    type name##_array_pop(name##_array_t *array) {                      \
        if (array->size == 0) {                                         \
            panic("[array_pop] can't pop an empty array");              \
        }                                                               \
                                                                        \
        return array->elements[--array->size];                          \
    }

#endif
