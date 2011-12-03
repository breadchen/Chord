#include <stdlib.h>
#include <string.h>

#include "node_struct.h"
#include "node_rpc.h"

static struct key* find_predecessor_local(const identifier id, void* arg);
static void init_node_data(struct node* n);
static struct key* get_closest_preceding_finger_local(const identifier id, struct node* finder);

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
		return find_predecessor_local(id, finder); 
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

/*
 * result will saved in finder->data
 */
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

	return ((struct node*)finder->data)->node_key;
}

/*
 * result saved in arg->data
 */
static struct key* find_predecessor_local(const identifier id, void* arg)
{
	struct node* node_in = (struct node*)arg;
	struct node* node_tmp;
	struct key* key_tmp = node_in->successor;

	if (node_in->data == NULL)
		init_node_data(node_in);
	node_tmp = node_in->data;

	memcpy(node_tmp->node_key, node_in->node_key, sizeof(struct key));
	while (!identifier_is_between(id, node_tmp->node_key->id, key_tmp->id, 0, 1))
	{ 
		key_tmp = get_closest_preceding_finger(id, node_tmp); // frist time node_tmp is local node but still use rpc function
															  // this is unnecessary but fix it makes code ugly ~ ~. maybe i
															  // will fix it with a better solution.
		memcpy(node_tmp->node_key, key_tmp, sizeof(struct key));
		key_tmp = get_node_successor(node_tmp);
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
