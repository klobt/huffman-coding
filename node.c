#include <stdio.h>
#include <stdlib.h>
#include "node.h"

node_t *node_create_leaf(char byte, unsigned int weight) {
    node_t *node;

    node = malloc(sizeof(node));

    node->leaf.type = NODE_LEAF;
    node->leaf.weight = weight;
    node->leaf.byte = byte;

    return node;
}

node_t *node_create_branch(node_t *left, node_t *right) {
    node_t *node;

    node = malloc(sizeof(node));

    node->branch.type = NODE_BRANCH;
    node->branch.weight = left->base.weight + right->base.weight;
    node->branch.left = left;
    node->branch.right = right;

    return node;
}

void node_free(node_t *root) {
    switch (root->base.type) {
        case NODE_LEAF:
            free(root);
            break;
        case NODE_BRANCH:
            node_free(root->branch.left);
            node_free(root->branch.right);
            free(root);
            break;
    }
}

void node_print(node_t *root) {
    switch (root->base.type) {
        case NODE_LEAF:
            putchar(root->leaf.byte);
            break;
        case NODE_BRANCH:
            putchar('(');
            node_print(root->branch.left);
            putchar(' ');
            node_print(root->branch.right);
            putchar(')');
            break;
    }
}
