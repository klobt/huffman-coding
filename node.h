#ifndef KLOBT_HUFFMAN_CODING__NODE_H_
#define KLOBT_HUFFMAN_CODING__NODE_H_

#include <stdio.h>
#include "node_breadcrumb_array.h"

typedef enum {
    NODE_LEAF,
    NODE_BRANCH,
} node_type_t;

typedef struct {
    node_type_t type;
    unsigned int weight;
} base_node_t;

typedef struct {
    node_type_t type;
    unsigned int weight;
    char byte;
} leaf_node_t;

typedef union node node_t;

typedef struct {
    node_type_t type;
    unsigned int weight;
    node_t *left, *right;
} branch_node_t;

union node {
    base_node_t base;
    leaf_node_t leaf;
    branch_node_t branch;
};

node_t *node_create_leaf(char byte, unsigned int weight);
node_t *node_create_branch(node_t *left, node_t *right);
void node_free(node_t *root);
void node_print(const node_t *root);
void node_encode(const node_t *root, FILE *stream);
int node_decode(FILE *stream, node_t **tree);
void node_traverse(const node_t *root, void (*callback)(const leaf_node_t *leaf, const node_breadcrumb_array_t *breadcrumbs));

#endif
