#include <stdio.h>
#include "node.h"

int main(int argc, char *argv[]) {
    node_t *tree = node_create_branch(
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

    node_free(tree);

    return 0;
}
