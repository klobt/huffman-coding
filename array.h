#ifndef KLOBT_HUFFMAN_CODING__ARRAY_
#define KLOBT_HUFFMAN_CODING__ARRAY_

#include <stddef.h>

#define DEFINE_ARRAY(name, type)                                        \
    typedef struct {                                                    \
        type *elements;                                                 \
        size_t capacity;                                                \
        size_t size;                                                    \
    } name##_array_t;                                                   \
                                                                        \
    name##_array_t *name##_array_create();                              \
    void name##_array_free(name##_array_t *array);                      \
    void name##_array_add(name##_array_t *array, type new_element);

DEFINE_ARRAY(char, char)

#endif
