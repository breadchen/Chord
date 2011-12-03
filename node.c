#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "node_struct.h"
#include "node_rpc.h"
#include "node_internal.h"

static void finger_table_new(struct node* node_local);
static void finger_node_free(struct node* finger_node);

struct node* node_new_local(const struct key* node_key)
{
	struct node* node_result = malloc(sizeof(struct node));

	node_result->node_key = malloc(sizeof(struct key));

	memcpy(node_result->node_key, node_key, sizeof(struct key));	
	node_result->node_socket = -1;
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
	node_result->node_socket = -1;
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
	for (counter = 0; counter < ID_BIT_LEN; counter++)
		finger_node_free(&(n->finger_table[counter].finger_node));
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

static struct node* finger_node_new(struct node* finger_node)
{
	finger_node->node_key = malloc(sizeof(struct key));
	finger_node->node_socket = -1;
	finger_node->predecessor = NULL;
	finger_node->successor = NULL;
	finger_node->finger_table = NULL;
	finger_node->data = NULL;
	finger_node->flag = NODE_FLAG_FINGER_NODE;

	return finger_node;
}

static void finger_node_free(struct node* finger_node)
{
	free(finger_node->node_key);
}

static void finger_table_new(struct node* node_local)
{
	int counter;
	struct finger* finger_table = node_local->finger_table;
	struct key* key_tmp;
	struct key* key_base = node_local->node_key;

	for (counter = 0; counter < ID_BIT_LEN; counter++)
	{
		finger_node_new(&(node_local->finger_table[counter].finger_node));

		key_tmp = (struct key*)&(node_local->finger_table[counter].start);
		key_generate_power(counter, key_tmp);
		key_add(key_tmp, key_base);
		memcpy(&(node_local->finger_table[counter].end), key_tmp, sizeof(identifier));
	}
}

static void init_finger_table(struct node* node_local, const struct node* node_exist)
{
	if ((node_local->flag & NODE_FLAG_LOCAL) == 0)
		return;

}
