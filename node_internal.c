#include <stdio.h>
#include <string.h>

#include "node_struct.h"
#include "node_rpc.h"

static struct key* find_predecessor_local(const identifier id, void* arg);
static void init_node_data(struct node* n);
static struct key* get_closest_preceding_finger_local(id, finder);

struct key* get_node_successor(const struct node* n)
{
	if ((n->flag & NODE_FLAG_LOCAL) != 0)
		return n->successor;
	else
		return rpc_get_successor(n);
}

struct key* get_node_predecessor(const struct node* n)
{
	if ((n->flag & NODE_FLAG_LOCAL) != 0)
		return n->predecessor;
	else
		return rpc_get_predecessor(n);
}

struct key* find_predecessor(const identifier id, void* arg)
{
	struct node* finder = (struct node*)arg;	

	if ((finder->flag & NODE_FLAG_LOCAL) != 0)	
	{
		if (finder->data == NULL)
			init_node_data(finder);

		return find_predecessor_local(id, finder); 
	}
	else
		return rpc_find_predecessor(id, arg);
}

struct key* find_successor(const identifier id, void* arg)
{
	struct node* finder = (struct node*)arg;

	if ((finder->flag & NODE_FLAG_LOCAL) != 0)
	{
		find_predecessor_local(id, finder);
		return ((struct node*)finder->data)->successor;
	}
	else
		return rpc_find_successor(id, arg);
}

struct key* get_closest_preceding_finger(const identifier id, struct node* finder)
{
	if ((finder->flag & NODE_FLAG_LOCAL) != 0)
		return get_closest_preceding_finger_local(id, finder);
	else
		return rpc_get_closest_preceding_finger(id, finder);
}

/*-------------- functions below only used in this file -------------------*/

static struct key* 
get_closest_preceding_finger_local(const identifier id, struct node* finder)
{
	int counter = ID_BIT_LEN - 1;
	struct finger* finger_t = finder->finger_table;
	struct node* node_current;

	for (; counter >= 0; counter--)
	{
		node_current = &(finger_t[counter].finger_node);
		if (identifier_is_between(node_current->node_key->id, 
								  finder->node_key->id,
								  id, 0, 0))
		{
			memcpy(((struct node*)finder->data)->node_key, 
				   node_current->node_key, sizeof(struct key));
		}
	} // end of for

	return ((struct node)finder->data)->node_key;
}

static struct key* find_predecessor_local(const identifier id, void* arg);
{
	struct node* node_in = (struct node*)arg;
	struct node* node_tmp = node_in->data;
	struct key* successor_tmp = node_in->successor;

	memcpy(node_tmp->node_key, node_in->node_key, sizeof(struct key));
	while (!identifier_is_between(id, node_tmp->node_key->id, successor_tmp->id, 0, 1))
	{
		get_closed_preceding_finger(id, node_in);
		successor_tmp = get_node_successor(node_tmp);
	} // end of while

	return node_tmp->node_key;
}

static void init_node_data(struct node* n)
{
	struct node* n_tmp;
	
	n->data = malloc(sizeof(struct node));
	n_tmp = (struct node*)n->data;
	n_tmp->node_key = malloc(sizeof(struct key));
	n_tmp->node_socket = -1;
	n_tmp->successor = malloc(sizeof(struct key));
	n_tmp->predecessor = malloc(sizeof(struct key));
	n_tmp->finger_table = malloc(sizeof(struct finger));
	n_tmp->data= NULL;
	n_tmp->flag = 0;
}
