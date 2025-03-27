#ifndef KLOBT_HUFFMAN_CODING__NODE_BREADCRUMB_ARRAY_H_
#define KLOBT_HUFFMAN_CODING__NODE_BREADCRUMB_ARRAY_H_

#include "array.h"

typedef enum {
    NODE_BREADCRUMB_LEFT,
    NODE_BREADCRUMB_RIGHT,
} node_breadcrumb_t;

DEFINE_ARRAY(node_breadcrumb, node_breadcrumb_t)

#endif
