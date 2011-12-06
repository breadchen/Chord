#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "node_struct.h"
#include "node_rpc.h"
#include "node_internal.h"

struct node* node_new_local(const struct key* node_key)
{
	struct node* node_result = malloc(sizeof(struct node));

	node_result->node_key = malloc(sizeof(struct key));

	memcpy(node_result->node_key, node_key, sizeof(struct key));	
	node_result->successor = malloc(sizeof(struct key));
	node_result->predecessor = malloc(sizeof(struct key));
	node_result->finger_table = malloc(ID_BIT_LEN * sizeof(struct finger));
	node_result->data = NULL;
	node_result->flag = NODE_FLAG_LOCAL;

	finger_table_new(node_result);

	return node_result;
}

/* we only use node_key and node_socket field in remote node struct
 *    other fields like successor,predecessor,finger_table are used as buffers */
struct node* node_new_remote(const struct key* node_key)
{
	struct node* node_result = malloc(sizeof(struct node));

	node_result->node_key = malloc(sizeof(struct key));

	memcpy(node_result->node_key, node_key, sizeof(struct key));	
	// use successor,predecessor,finger_table as buffer
	node_result->successor = malloc(sizeof(struct key));
	node_result->predecessor = malloc(sizeof(struct key));
	node_result->finger_table = malloc(sizeof(struct key));
	node_result->data = NULL;
	node_result->flag = 0;

	return node_result;
}

void node_free(struct node* n)
{
	int counter;

	free(n->node_key);
	free(n->successor);
	free(n->predecessor);
	free(n->finger_table);
	// only for local node
	if(n->data != NULL)
	{
		free(((struct node*)(n->data))->node_key);
		free(((struct node*)(n->data))->successor);
		free(((struct node*)(n->data))->predecessor);
		free(((struct node*)(n->data))->finger_table);		
		free(((struct node*)(n->data))->data);
	}
	free(n);
}

static void finger_table_new(struct node* node_local)
{
	int counter;
	struct finger* finger_table = node_local->finger_table;
	struct key* key_tmp;
	struct key* key_base = node_local->node_key;

	for (counter = 0; counter < ID_BIT_LEN; counter++)
	{
		key_tmp = (struct key*)&(node_local->finger_table[counter].start);
		key_generate_power(counter, key_tmp);
		key_add(key_tmp, key_base);
		memcpy(&(node_local->finger_table[counter].end), key_tmp, sizeof(identifier));
	} // end of for
}

static void init_finger_table(struct node* node_local, const struct node* node_exist)
{
	struct node* node_tmp;
	struct key* successor;
	struct key* predecessor;
	int counter;

	if ((node_local->flag & NODE_FLAG_LOCAL) == 0)
		return;

	node_tmp = node_remote_new();
	*(node_tmp->node_key) = *(node_exist->node_key);

	predecessor = rpc_find_predecessor(node_local->node_key.addr, node_tmp);
	*(node_tmp->node_key) = predecessor;
	successor = rpc_get_successor(&node_tmp);

	// insert node_local
	*(node_local->predecessor) = *predecessor;
	*(node_local->successor) = *successor;
	*(node_tmp->node_key) = *predecessor;
	rpc_set_successor(&node_tmp, node_local->node_key);
	*(node_tmp->node_key) = *successor;
	rpc_set_predecessor(&node_tmp, node_local->node_key);

	// init finger table
	*(node_tmp->node_key) = *(node_exist->node_key);
	node_local->finger_table[0].successor = *successor
	for (counter = 0; counter < ID_BIT_LEN - 1; counter++)
	{
		if (key_is_between(&(node_local->finger_table[counter+1].start), 
						   node_local->node_key, 
						   node_local->finger_table[counter].successor))
			node_local->finger_table[counter+1].successor = 
				node_local->finger_table[counter].successor;
		else
			node_local->finger_table[counter+1].successor = 
				*(rpc_find_successor(node_local->finger_table[counter].start, node_tmp));
	} // end of for

	node_free(node_tmp);
}

static void update_others(struct node* node_local)
{
	int counter;
	struct key* p;
	struct key key_tmp;
	struct key key_power;

	for (counter = 0; counter < ID_BIT_LEN; counter++)
	{
		key_tmp = *(node_local->node_key);
		key_generate_power(counter, &key_power);
		key_minus(&key_tmp, &key_power);

		p = find_predecessor(key_tmp.id, node_local);
		rpc_update_finger_table(p, node_local->node_key, counter);
	} // end of for
}

static void update_finger_table(struct node* node_local, 
								const struct key* finger_key, 
								int i)
{
	struct key* finger_key = node_local->finger_table[i].successor;
	if (key_is_between(finger_key, node_local, finger_key, true, false))
	{
		node_local->finger_table[i].successor = *finger_key;
		rpc_update_finger_table(node_local->predecessor, finger_key, i);
	}
}
