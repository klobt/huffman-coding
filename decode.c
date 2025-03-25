#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "binary.h"
#include "config.h"
#include "node.h"
#include "utilities.h"

int main(int argc, char *argv[]) {
    FILE *input_file, *output_file;
    node_t *tree, *node;
    char total_bytes[8];
    unsigned long long total;
    binary_reader_t *reader;

    parse_arguments(argc, argv, DECODE_PROGRAM_NAME, &input_file, &output_file);

    if (node_decode(input_file, &tree) < 0) {
        fprintf(stderr, "%s: Failed to decode the tree.\n", DECODE_PROGRAM_NAME);
        return EXIT_FAILURE;
    }

    if (fread(total_bytes, sizeof(char), 8, input_file) != 8) {
        perror(DECODE_PROGRAM_NAME);
        return EXIT_FAILURE;
    }

    total = little_endian_to_long(total_bytes);

    reader = binary_reader_create(input_file);
    for (unsigned long long index = 0; index < total; index++) {
        node = tree;
        while (node != NULL && node->base.type == NODE_BRANCH) {
            switch (binary_read(reader)) {
                case BINARY_ZERO:
                    node = node->branch.left;
                    break;
                case BINARY_ONE:
                    node = node->branch.right;
                    break;
                case BINARY_END_OF_BUFFER:
                    fprintf(stderr, "%s: Unexpected end of buffer.\n", DECODE_PROGRAM_NAME);
                    return EXIT_FAILURE;
            }
        }
        if (node == NULL) {
            fprintf(stderr, "%s: Unexpected bit.\n", DECODE_PROGRAM_NAME);
            return EXIT_FAILURE;
        }
        putc(node->leaf.byte, output_file);
    }
    binary_reader_free(reader);

    node_free(tree);

    close_files(input_file, output_file);

    return EXIT_SUCCESS;
}
