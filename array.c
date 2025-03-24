#include <stdlib.h>
#include "array.h"

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
    void name##_array_add(name##_array_t *array, type new_element) {    \
        if (array->size == array->capacity) {                           \
            array->capacity *= 2;                                       \
            array->elements = realloc(                                  \
                array->elements,                                        \
                sizeof(type) * array->capacity \
            );                                                          \
        }                                                               \
                                                                        \
        array->elements[array->size++] = new_element;                   \
    }

IMPLEMENT_ARRAY(char, char)
