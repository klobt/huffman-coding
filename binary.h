#ifndef KLOBT_HUFFMAN_CODING__BINARY_
#define KLOBT_HUFFMAN_CODING__BINARY_

#include "array.h"
#include "stddef.h"

typedef struct {
    const char_array_t *buffer;
    size_t bit_position;
} binary_reader_t;

typedef struct {
    char_array_t *buffer;
    size_t bit_position;
} binary_writer_t;

typedef enum {
    BINARY_ZERO,
    BINARY_ONE,
    BINARY_END_OF_BUFFER,
} binary_result_t;

binary_result_t binary_read(binary_reader_t *reader);
void binary_write(binary_writer_t *writer, unsigned int bit);

#endif
