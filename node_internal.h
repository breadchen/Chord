#ifndef _NODE_INTERNAL_H
#define _NODE_INTERNAL_H

#include "key.h"

struct key* get_node_successor(const struct node* n);

struct key* get_node_predecessor(const struct node* n);

#endif
