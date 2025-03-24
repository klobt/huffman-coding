#include <stdio.h>
#include "array.h"
#include "binary.h"
#include "node.h"
#include "node_queue.h"

int main(int argc, char *argv[]) {
    node_t *tree, *new_tree;
    char_array_t *out_array, *in_array, *tree_buffer;
    binary_reader_t reader;
    binary_writer_t writer;
    node_queue_t *queue;

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

    tree_buffer = char_array_create();
    node_encode(tree, tree_buffer);
    node_decode(tree_buffer, 0, &new_tree);
    node_print(new_tree);
    putchar('\n');
    node_free(new_tree);
    char_array_free(tree_buffer);

    node_free(tree);

    out_array = char_array_create();
    in_array = char_array_create();

    char_array_add(out_array, 'a');
    char_array_add(out_array, 'b');
    char_array_add(out_array, 'b');
    char_array_add(out_array, 'a');
    char_array_add(out_array, '\0');

    reader.buffer = out_array;
    reader.bit_position = 0;

    writer.buffer = in_array;
    writer.bit_position = 0;

    for (binary_result_t r = binary_read(&reader); r != BINARY_END_OF_BUFFER; r = binary_read(&reader)) {
        printf("%d", r);
        binary_write(&writer, r);
    }
    putchar('\n');

    printf("%s\n", in_array->elements);

    char_array_free(out_array);
    char_array_free(in_array);

    queue = node_queue_create();

    node_queue_insert(queue, node_create_leaf('_', 10));
    node_queue_insert(queue, node_create_leaf('D', 10));
    node_queue_insert(queue, node_create_leaf('A', 11));
    node_queue_insert(queue, node_create_leaf('E', 7));
    node_queue_insert(queue, node_create_leaf('C', 2));
    node_queue_insert(queue, node_create_leaf('B', 6));

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

    node_print(tree);
    putchar('\n');

    node_free(tree);

    return 0;
}
