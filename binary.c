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

    if (reader->bit_position / 8 >= reader->buffer_size) {
        return BINARY_END_OF_BUFFER;
    }

    result = read_bit(reader->buffer[reader->bit_position / 8], reader->bit_position % 8);

    reader->bit_position++;

    return result;
}

binary_result_t binary_write(binary_writer_t *writer, unsigned int bit) {
    if (writer->bit_position / 8 >= writer->buffer_size) {
        return BINARY_END_OF_BUFFER;
    }

    write_bit(&writer->buffer[writer->bit_position / 8], writer->bit_position % 8, bit);

    writer->bit_position++;

    return bit % 2;
}
