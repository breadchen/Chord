#ifndef _NODE_H
#define _NODE_H

#include "key.h"

struct options;
struct finger;

struct node
{
	struct key node_key;
	struct finger* finger_table;
	struct options ops;
};

struct finger
{
	Identifier start;	
}

#endif
