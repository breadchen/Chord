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
	node_result->node_socket = -1;
	node_result->successor = malloc(sizeof(struct key));
	node_result->predecessor = malloc(sizeof(struct key));
	node_result->finger_table = malloc(ID_BIT_LEN * sizeof(struct finger));
	node_result->data = NULL;
	node_result->flag = NODE_FLAG_LOCAL;
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
}

void node_free(struct node* n)
{
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

static void init_finger_table(struct node* node_local, const struct node* node_exist)
{
	if ((node_local->flag & NODE_FLAG_LOCAL) == 0)
		return;

}
