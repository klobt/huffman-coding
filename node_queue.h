#ifndef KLOBT_HUFFMAN_CODING__NODE_QUEUE_H_
#define KLOBT_HUFFMAN_CODING__NODE_QUEUE_H_

#include "node_array.h"

typedef struct {
    node_array_t *array;
} node_queue_t;

node_queue_t *node_queue_create(void);
void node_queue_free(node_queue_t *queue);
void node_queue_insert(node_queue_t *queue, node_t *node);
node_t *node_queue_pop_minimum(node_queue_t *queue);

#endif
