#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "config.h"
#include "node.h"
#include "utilities.h"

int main(int argc, char *argv[]) {
    FILE *input_file, *output_file;
    node_t *tree;
    char total_bytes[8];

    parse_arguments(argc, argv, DECODE_PROGRAM_NAME, &input_file, &output_file);

    if (node_decode(input_file, &tree) < 0) {
        fprintf(stderr, "%s: Failed to decode the tree.\n", DECODE_PROGRAM_NAME);
        return EXIT_FAILURE;
    }

    if (fread(total_bytes, sizeof(char), 8, input_file) != 8) {
        perror(DECODE_PROGRAM_NAME);
        return EXIT_FAILURE;
    }

    node_print(tree);
    putc('\n', stderr);

    fprintf(stderr, "%llu\n", little_endian_to_long(total_bytes));

    node_free(tree);

    close_files(input_file, output_file);

    return EXIT_SUCCESS;
}
