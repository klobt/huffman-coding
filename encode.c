#include <stdio.h>
#include "binary.h"
#include "node.h"

int main(int argc, char *argv[]) {
    node_t *tree;
    int a, b;
    binary_reader_t reader;
    binary_writer_t writer;

    tree = node_create_branch(
        node_create_branch(
            node_create_leaf('_', 10),
            node_create_leaf('D', 10)
        ),
        node_create_branch(
            node_create_leaf('A', 11),
            node_create_branch(
                node_create_leaf('E', 7),
                node_create_branch(
                    node_create_leaf('C', 2),
                    node_create_leaf('B', 6)
                )
            )
        )
    );

    node_print(tree);
    putchar('\n');

    node_free(tree);

    a = 13;

    reader.buffer = (const char *) &a;
    reader.buffer_size = sizeof(a);
    reader.bit_position = 0;

    writer.buffer = (char *) &b;
    writer.buffer_size = sizeof(b);
    writer.bit_position = 0;

    for (binary_result_t r = binary_read(&reader); r != BINARY_END_OF_BUFFER; r = binary_read(&reader)) {
        printf("%d", r);
    }
    putchar('\n');

    printf("%d\n", b);

    return 0;
}
