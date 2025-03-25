#include <stddef.h>
#include <stdlib.h>
#include "node_queue.h"

#define SWAP(x, y)                                              \
    {                                                           \
        x = (node_t *) ((unsigned long long) x ^ (unsigned long long) y); \
        y = (node_t *) ((unsigned long long) x ^ (unsigned long long) y); \
        x = (node_t *) ((unsigned long long) x ^ (unsigned long long) y); \
    }

node_queue_t *node_queue_create() {
    node_queue_t *queue;

    queue = malloc(sizeof(node_queue_t));
    queue->array = node_array_create();

    return queue;
}

void node_queue_free(node_queue_t *queue) {
    node_array_free(queue->array);
    free(queue);
}

size_t parent(size_t index) {
    return (index - 1) / 2;
}

size_t left(size_t index) {
    return index * 2 + 1;
}

size_t right(size_t index) {
    return index * 2 + 2;
}

unsigned int weight(node_queue_t *queue, size_t index) {
    return queue->array->elements[index]->base.weight;
}

void node_queue_insert(node_queue_t *queue, node_t *node) {
    size_t index, parent_index;

    node_array_add(queue->array, node);
    index = queue->array->size - 1;

    parent_index = parent(index);

    while (index > 0 && weight(queue, index) < weight(queue, parent_index)) {
        SWAP(queue->array->elements[index], queue->array->elements[parent_index]);
        index = parent_index;
        parent_index = parent(index);
    }
}

void heapify(node_queue_t *queue, size_t index) {
    size_t minimum_index, left_index, right_index;

    minimum_index = index;
    left_index = left(index);
    right_index = right(index);

    if (left_index >= queue->array->size) {
        return;
    }

    if (weight(queue, left_index) < weight(queue, index)) {
        minimum_index = left_index;
    }

    if (right_index < queue->array->size && weight(queue, right_index) < weight(queue, minimum_index)) {
        minimum_index = right_index;
    }

    if (minimum_index == index) {
        return;
    }

    SWAP(queue->array->elements[index], queue->array->elements[minimum_index]);

    heapify(queue, minimum_index);
}

node_t *node_queue_pop_minimum(node_queue_t *queue) {
    node_t *minimum;

    if (queue->array->size == 0) {
        return NULL;
    } else if (queue->array->size == 1) {
        return node_array_pop(queue->array);
    }

    minimum = queue->array->elements[0];
    queue->array->elements[0] = node_array_pop(queue->array);

    heapify(queue, 0);

    return minimum;
}
