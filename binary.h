#ifndef KLOBT_HUFFMAN_CODING__BINARY_
#define KLOBT_HUFFMAN_CODING__BINARY_

#include <stdio.h>
#include "char_array.h"
#include "stddef.h"

typedef struct {
    FILE *stream;
    char current;
    size_t bit_position;
    char_array_t *buffer;
} binary_reader_t;

typedef struct {
    FILE *stream;
    char current;
    size_t bit_position;
} binary_writer_t;

typedef enum {
    BINARY_ZERO,
    BINARY_ONE,
    BINARY_END_OF_BUFFER,
} binary_result_t;

binary_reader_t *binary_reader_create(FILE *stream);
void binary_reader_free(binary_reader_t *reader);
binary_writer_t *binary_writer_create(FILE *stream);
void binary_writer_free(binary_writer_t *writer);
binary_result_t binary_read(binary_reader_t *reader);
void binary_write(binary_writer_t *writer, unsigned int bit);
void binary_pad(binary_writer_t *writer, unsigned int bit);

#endif
