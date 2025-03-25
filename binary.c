#include "binary.h"
#include "utilities.h"

binary_reader_t *binary_reader_create(FILE *stream) {
    binary_reader_t *reader;

    reader = malloc(sizeof(binary_reader_t));
    reader->stream = stream;
    reader->bit_position = 0;
    reader->buffer = char_array_create();

    return reader;
}

void binary_reader_free(binary_reader_t *reader) {
    char_array_free(reader->buffer);
    free(reader);
}

binary_writer_t *binary_writer_create(FILE *stream) {
    binary_writer_t *writer;

    writer = malloc(sizeof(binary_writer_t));
    writer->stream = stream;
    writer->bit_position = 0;

    return writer;
}

void binary_writer_free(binary_writer_t *writer) {
    free(writer);
}

unsigned int read_bit(char byte, size_t bit_position) {
    if (bit_position >= 8) {
        panic("[select_bit] bit position is 8 or more");
    }

    return (byte >> bit_position) & 1;
}

void write_bit(char *byte, size_t bit_position, unsigned int bit) {
    if (bit_position >= 8) {
        panic("[select_bit] bit position is 8 or more");
    }

    if (bit > 0) {
        *byte |= 1 << bit_position;
    } else {
        *byte &= ~(1 << bit_position);
    }
}

binary_result_t binary_read(binary_reader_t *reader) {
    int getc_result;
    binary_result_t result;

    if (reader->bit_position / 8 >= reader->buffer->size) {
        getc_result = getc(reader->stream);
        if (getc_result == EOF) {
            return BINARY_END_OF_BUFFER;
        }

        char_array_add(reader->buffer, getc_result);
    }

    reader->current = reader->buffer->elements[reader->bit_position / 8];

    result = read_bit(reader->current, reader->bit_position % 8);

    reader->bit_position++;

    return result;
}

void binary_write(binary_writer_t *writer, unsigned int bit) {
    write_bit(&writer->current, writer->bit_position % 8, bit);

    writer->bit_position++;

    if (writer->bit_position % 8 == 0) {
        putc(writer->current, writer->stream);
    }
}

void binary_pad(binary_writer_t *writer, unsigned int bit) {
    while (writer->bit_position % 8 > 0) {
        write_bit(&writer->current, writer->bit_position % 8, bit);

        writer->bit_position++;
    }

    putc(writer->current, writer->stream);
}
