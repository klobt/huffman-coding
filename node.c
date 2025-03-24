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
    if (root == NULL) {
        putchar('N');
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

void node_encode_layout(node_t *root, binary_writer_t *writer) {
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

void node_encode_leaf_values(node_t *root, char_array_t *buffer) {
        switch (root->base.type) {
        case NODE_LEAF:
            char_array_add(buffer, root->leaf.byte);
            break;
        case NODE_BRANCH:
            node_encode_leaf_values(root->branch.left, buffer);
            node_encode_leaf_values(root->branch.right, buffer);
            break;
    }
}

void node_encode(node_t *root, char_array_t *buffer) {
    binary_writer_t writer;

    writer.buffer = buffer;
    writer.bit_position = buffer->size * 8;

    node_encode_layout(root, &writer);
    binary_pad(&writer, 0);
    node_encode_leaf_values(root, buffer);
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

node_t *node_decode_tree(binary_reader_t *reader, size_t *next_leaf_index) {
    node_t *left, *right;

    switch (binary_read(reader)) {
        case BINARY_ZERO:
            return node_create_leaf(reader->buffer->elements[(*next_leaf_index)++], 0);
        case BINARY_ONE:
            left = node_decode_tree(reader, next_leaf_index);
            right = node_decode_tree(reader, next_leaf_index);

            return node_create_branch(left, right);
        default:
            return NULL;
    }
}

size_t node_decode(char_array_t *buffer, size_t offset, node_t **tree) {
    binary_reader_t reader;
    size_t leaf_count;
    size_t next_leaf_index;

    reader.bit_position = offset * 8;

    leaf_count = node_decode_leaf_count(&reader);
    next_leaf_index = (reader.bit_position - 1) / 8 + 1;

    if (next_leaf_index + leaf_count > buffer->size) {
        return 0;
    }

    reader.bit_position = offset * 8;

    *tree = node_decode_tree(&reader, &next_leaf_index);

    return (reader.bit_position - 1) / 8 + 1;
}
