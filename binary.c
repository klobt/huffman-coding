#include "binary.h"
#include "utilities.h"

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
    binary_result_t result;

    if (reader->bit_position / 8 >= reader->buffer->size) {
        return BINARY_END_OF_BUFFER;
    }

    result = read_bit(reader->buffer->elements[reader->bit_position / 8], reader->bit_position % 8);

    reader->bit_position++;

    return result;
}

void binary_write(binary_writer_t *writer, unsigned int bit) {
    if (writer->bit_position / 8 >= writer->buffer->size) {
        char_array_add(writer->buffer, 0);
    }

    write_bit(&writer->buffer->elements[writer->bit_position / 8], writer->bit_position % 8, bit);

    writer->bit_position++;
}

void binary_pad(binary_writer_t *writer, unsigned int bit) {
    while (writer->bit_position % 8 > 0) {
        write_bit(&writer->buffer->elements[writer->bit_position / 8], writer->bit_position % 8, bit);

        writer->bit_position++;
    }
}
