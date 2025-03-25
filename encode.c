#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "node_queue.h"
#include "node_breadcrumb_array.h"

#define FILE_BUFFER_SIZE 1024

void count_bytes(FILE *file, unsigned int byte_counts[256]) {
    char file_buffer[FILE_BUFFER_SIZE];
    size_t bytes_read;

    for (size_t byte = 0; byte < 256; byte++) {
        byte_counts[byte] = 0;
    }

    while ((bytes_read = fread(file_buffer, sizeof(char), FILE_BUFFER_SIZE, file)) != 0) {
        for (size_t index = 0; index < bytes_read; index++) {
            byte_counts[(size_t) file_buffer[index]]++;
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
    byte_codes[(size_t) leaf->byte] = node_breadcrumb_array_clone(breadcrumbs);
}

void retrieve_byte_codes(const node_t *root) {
    for (size_t byte = 0; byte < 256; byte++) {
        byte_codes[byte] = NULL;
    }

    node_traverse(root, retrieve_byte_code_traverse_callback);
}

int main(int argc, char *argv[]) {
    FILE *input_file, *output_file;
    unsigned int byte_counts[256];
    node_t *tree;

    parse_arguments(argc, argv, DECODE_PROGRAM_NAME, &input_file, &output_file);

    count_bytes(input_file, byte_counts);

    tree = create_tree_from_byte_counts(byte_counts);

    node_encode(tree, output_file);

    retrieve_byte_codes(tree);

    node_free(tree);

    for (size_t byte = 0; byte < 256; byte++) {
        if (byte_codes[byte] != NULL) {
            node_breadcrumb_array_free(byte_codes[byte]);
        }
    }

    close_files(input_file, output_file);

    return EXIT_SUCCESS;
}
