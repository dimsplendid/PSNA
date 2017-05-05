#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "data_struct.h"
#include "aux.h"

static int node_print_impl(Node *);
static int node_free_impl(Node *);

int init_Node(Node ** self){
	if ( NULL == (*self = malloc(sizeof(Node)))) {
		fprintf(stderr,"Error in graph.c (init_Node): %s \n", strerror(errno));
		exit(errno);
	}
	(*self)->id = -1;
	(*self)->label = NULL;
	(*self)->data = 0.0;
	(*self)->print = node_print_impl;
	(*self)->free = node_free_impl;
	return 0;
}

static int node_print_impl(Node * self){
	printf("Node[%d]: %f",self->id,self->data);
	return 0;
}

static int node_free_impl(Node * self){
	free(self);
	return 0;
}

static int edge_free_impl(Edge * self);
static int edge_print_impl(Edge * self);

int init_Edge(Edge ** self){
	if(NULL == (*self = malloc(sizeof(Edge)))){
		fprintf(stderr,"Error in graph.c (init_Edge): %s\n", strerror(errno));
		exit(errno);
	}

	(*self)->id = -1;
	(*self)->s = NULL;
	(*self)->t = NULL;
	(*self)->c = 0.0;
	(*self)->f = 0.0;

	(*self)->print = edge_print_impl;
	(*self)->free = edge_free_impl;
	return 0;
}

static int edge_print_impl(Edge * self){
	printf("edge[%d]: ",self->id);
	Edge->s->print(Edge->s);
	printf(" -> ");
	Edge->t->print(Edge->t);
	return 0;
}

static int edge_free_impl(Edge * self){
	self->s->free(self->s);
	self->s->free(self->t);
	free(self);
}

static graph_print_impl(Graph * self);
static graph_free_impl(Graph * self);
static graph_addEdge_impl(Graph * self, Node * s, Node * t);
static Node * graph_getNode_impl(Graph * self, int id);
static Edge * graph_getEdge_impl(Graph * self, int id);


