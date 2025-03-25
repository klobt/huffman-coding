#include <stdio.h>
#include <stdlib.h>
#include "binary.h"
#include "config.h"
#include "char_array.h"
#include "node_queue.h"
#include "node_breadcrumb_array.h"

#define FILE_BUFFER_SIZE 1024

void count_bytes(FILE *file, char_array_t *buffer, unsigned int byte_counts[256]) {
    char file_buffer[FILE_BUFFER_SIZE];
    size_t bytes_read;

    for (size_t byte = 0; byte < 256; byte++) {
        byte_counts[byte] = 0;
    }

    while ((bytes_read = fread(file_buffer, sizeof(char), FILE_BUFFER_SIZE, file)) != 0) {
        for (size_t index = 0; index < bytes_read; index++) {
            byte_counts[(size_t) (unsigned char) file_buffer[index]]++;
            char_array_add(buffer, file_buffer[index]);
        }
    }

    if (ferror(file)) {
        perror(ENCODE_PROGRAM_NAME);
        fclose(file);

        exit(EXIT_FAILURE);
    }
}

node_t *create_tree_from_byte_counts(const unsigned int byte_counts[256]) {
    node_queue_t *queue;
    node_t *tree;

    queue = node_queue_create();

    for (size_t byte = 0; byte < 256; byte++) {
        if (byte_counts[byte] > 0) {
            node_queue_insert(queue, node_create_leaf((char) byte, byte_counts[byte]));
        }
    }

    while (queue->array->size > 1) {
        node_queue_insert(
            queue,
            node_create_branch(
                node_queue_pop_minimum(queue),
                node_queue_pop_minimum(queue)
            )
        );
    }

    tree = node_queue_pop_minimum(queue);

    node_queue_free(queue);

    return tree;
}

node_breadcrumb_array_t *byte_codes[256];

void retrieve_byte_code_traverse_callback(const leaf_node_t *leaf, const node_breadcrumb_array_t *breadcrumbs) {
    byte_codes[(size_t) (unsigned char) leaf->byte] = node_breadcrumb_array_clone(breadcrumbs);
}

void retrieve_byte_codes(const node_t *root) {
    for (size_t byte = 0; byte < 256; byte++) {
        byte_codes[byte] = NULL;
    }

    node_traverse(root, retrieve_byte_code_traverse_callback);
}

int main(int argc, char *argv[]) {
    FILE *input_file, *output_file;
    char_array_t *input_file_buffer;
    unsigned int byte_counts[256];
    node_t *tree;
    char total_bytes[8];
    binary_writer_t *writer;
    node_breadcrumb_array_t *breadcrumbs;

    parse_arguments(argc, argv, DECODE_PROGRAM_NAME, &input_file, &output_file);

    input_file_buffer = char_array_create();
    count_bytes(input_file, input_file_buffer, byte_counts);

    tree = create_tree_from_byte_counts(byte_counts);

    node_encode(tree, output_file);

    long_to_little_endian(input_file_buffer->size, total_bytes);
    if (fwrite(total_bytes, sizeof(char), 8, output_file) != 8) {
        perror(ENCODE_PROGRAM_NAME);
        return EXIT_FAILURE;
    }

    retrieve_byte_codes(tree);

    writer = binary_writer_create(output_file);
    for (unsigned long long index = 0; index < input_file_buffer->size; index++) {
        breadcrumbs = byte_codes[(size_t) (unsigned char) input_file_buffer->elements[index]];
        for (size_t breadcrumb_index = 0; breadcrumb_index < breadcrumbs->size; breadcrumb_index++) {
            binary_write(writer, breadcrumbs->elements[breadcrumb_index]);
        }
    }
    binary_pad(writer, 0);
    binary_writer_free(writer);

    node_free(tree);

    for (size_t byte = 0; byte < 256; byte++) {
        if (byte_codes[byte] != NULL) {
            node_breadcrumb_array_free(byte_codes[byte]);
        }
    }

    char_array_free(input_file_buffer);

    close_files(input_file, output_file);

    return EXIT_SUCCESS;
}
