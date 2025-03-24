#ifndef KLOBT_HUFFMAN_CODING__NODE_
#define KLOBT_HUFFMAN_CODING__NODE_

typedef enum {
    NODE_LEAF,
    NODE_BRANCH,
} node_type_t;

typedef struct {
    node_type_t type;
    unsigned int weight;
} node_base_t;

typedef struct {
    node_type_t type;
    unsigned int weight;
    char byte;
} node_leaf_t;

typedef union node node_t;

typedef struct {
    node_type_t type;
    unsigned int weight;
    node_t *left, *right;
} node_branch_t;

union node {
    node_base_t base;
    node_leaf_t leaf;
    node_branch_t branch;
};

node_t *node_create_leaf(char byte, unsigned int weight);
node_t *node_create_branch(node_t *left, node_t *right);
void node_free(node_t *root);
void node_print(node_t *root);

#endif
