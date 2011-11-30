#ifndef _NODE_H
#define _NODE_H

#include "node_struct.h"

struct node* node_new_local(const struct key* node_key);

void node_free(struct node* n);

int node_join(struct node* node_fresh, const struct node* node_exist);

int node_exit(struct node* n);

int node_send(struct node* node_sender, struct node* node_dst, char* data, int len);

int node_receive(struct node* node_receiver, struct node* node_from, char* buf, int len);

#endif
