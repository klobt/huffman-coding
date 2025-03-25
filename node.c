#include <stdio.h>
#include <stdlib.h>
#include "binary.h"
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
    unsigned int left_weight, right_weight;

    left_weight = left != NULL ? left->base.weight : 0;
    right_weight = right != NULL ? right->base.weight : 0;

    node = malloc(sizeof(node));

    node->branch.type = NODE_BRANCH;
    node->branch.weight = left_weight + right_weight;
    node->branch.left = left;
    node->branch.right = right;

    return node;
}

void node_free(node_t *root) {
    if (root == NULL) {
        return;
    }

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

void node_print(const node_t *root) {
    if (root == NULL) {
        putchar('N');

        return;
    }

    switch (root->base.type) {
        case NODE_LEAF:
            putchar('\'');
            putchar(root->leaf.byte);
            putchar('\'');
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

void node_encode_layout(const node_t *root, binary_writer_t *writer) {
        switch (root->base.type) {
        case NODE_LEAF:
            binary_write(writer, 0);
            break;
        case NODE_BRANCH:
            binary_write(writer, 1);
            node_encode_layout(root->branch.left, writer);
            node_encode_layout(root->branch.right, writer);
            break;
    }
}

void node_encode_leaf_values(const node_t *root, FILE *stream) {
    switch (root->base.type) {
        case NODE_LEAF:
            putc(root->leaf.byte, stream);
            break;
        case NODE_BRANCH:
            node_encode_leaf_values(root->branch.left, stream);
            node_encode_leaf_values(root->branch.right, stream);
            break;
    }
}

void node_encode(const node_t *root, FILE *stream) {
    binary_writer_t *writer;

    writer = binary_writer_create(stream);

    node_encode_layout(root, writer);
    binary_pad(writer, 0);
    node_encode_leaf_values(root, stream);

    binary_writer_free(writer);
}

size_t node_decode_leaf_count(binary_reader_t *reader) {
    switch (binary_read(reader)) {
        case BINARY_ZERO:
            return 1;
        case BINARY_ONE:
            return node_decode_leaf_count(reader) + node_decode_leaf_count(reader);
        default:
            return 0;
    }
}

node_t *node_decode_tree(binary_reader_t *reader, char **next_leaf) {
    node_t *left, *right;

    switch (binary_read(reader)) {
        case BINARY_ZERO:
            return node_create_leaf(*(*next_leaf)++, 0);
        case BINARY_ONE:
            left = node_decode_tree(reader, next_leaf);
            right = node_decode_tree(reader, next_leaf);

            return node_create_branch(left, right);
        default:
            return NULL;
    }
}

int node_decode(FILE *stream, node_t **tree) {
    binary_reader_t *reader;
    size_t leaf_count;
    int getc_result;
    char *leaves, *leaf_p;
    int result;

    reader = binary_reader_create(stream);

    leaf_count = node_decode_leaf_count(reader);
    leaves = malloc(sizeof(char) * leaf_count);

    for (size_t index = 0; index < leaf_count; index++) {
        getc_result = getc(stream);
        if (getc_result == EOF) {
             return -1;
        }
        leaves[index] = getc_result;
    }

    reader->bit_position = 0;

    leaf_p = leaves;
    *tree = node_decode_tree(reader, &leaf_p);

    result = (reader->bit_position - 1) / 8 + 1;

    binary_reader_free(reader);

    return result;
}

void traverse(const node_t *node, void (*callback)(const leaf_node_t *leaf, const node_breadcrumb_array_t *breadcrumbs), node_breadcrumb_array_t *breadcrumbs) {
    switch (node->base.type) {
        case NODE_LEAF:
            callback(&node->leaf, breadcrumbs);
            break;
        case NODE_BRANCH:
            node_breadcrumb_array_add(breadcrumbs, NODE_BREADCRUMB_LEFT);
            traverse(node->branch.left, callback, breadcrumbs);
            node_breadcrumb_array_pop(breadcrumbs);
            node_breadcrumb_array_add(breadcrumbs, NODE_BREADCRUMB_RIGHT);
            traverse(node->branch.right, callback, breadcrumbs);
            node_breadcrumb_array_pop(breadcrumbs);
            break;
    }
}

void node_traverse(const node_t *root, void (*callback)(const leaf_node_t *leaf, const node_breadcrumb_array_t *breadcrumbs)) {
    node_breadcrumb_array_t *breadcrumbs;

    breadcrumbs = node_breadcrumb_array_create();
    traverse(root, callback, breadcrumbs);
    node_breadcrumb_array_free(breadcrumbs);
}
