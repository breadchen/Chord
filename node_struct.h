#ifndef _NODE_STRUCT_H
#define _NODE_STRUCT_H

#include "key.h"

struct node
{
	/* node identifier and address */
	struct key* node_key;
	int node_socket;
	struct key* successor;
	struct key* predecessor;
	struct finger* finger_table;
	void* data;
	int flag;
};

struct finger
{
	identifier start;	
	identifier end;
	struct node finger_node;
};

#define NODE_FLAG_LOCAL 0x00000001
#define NODE_FLAG_JOINED 0x00000002
#define NODE_FLAG_FINGER_NODE 0x00000004

#endif
