#ifndef _NODE_RPC_H
#define _NODE_RPC_H

#include "node_struct.h"

struct key* rpc_find_successor(const identifier id, void* arg);

struct key* rpc_find_predecessor(const identifier id, void* arg);

struct key* rpc_get_successor(const struct node* node_remote);

struct key* rpc_get_predecessor(const struct node* node_remote);

struct key* rpc_get_closest_preceding_finger(const identifier id, struct node* node_remote);

struct key* rpc_set_successor(const struct node* node_remote, const struct key* successor);

struct key* rpc_set_precedessor(const struct node* node_remote, const struct key* predecessor);

void rpc_update_finger_table(const struct key* key_remote, 
		const struct key* finger_key, int i);

//struct key* rpc_response(struct node* node_remote, int code);

#endif
