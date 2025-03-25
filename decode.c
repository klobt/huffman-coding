#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "node.h"

void parse_arguments(int argc, char *argv[]) {
    if (argc != 1) {
        fprintf(stderr, "usage: %s\n", argv[0]);

        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    node_t *tree;

    parse_arguments(argc, argv);

    if (node_decode(stdin, &tree) < 0) {
        perror(DECODE_PROGRAM_NAME);

        return EXIT_FAILURE;
    }

    node_print(tree);
    putchar('\n');

    node_free(tree);

    return EXIT_SUCCESS;
}
