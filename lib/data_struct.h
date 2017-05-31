/*
* file: data_struct.h
* header file of list.c graph.c tree.c
* my first oop in c
*/

#pragma once
#include<stdlib.h>
#include<gsl/gsl_matrix.h>

typedef struct lst Lst; // dynamic link list
typedef struct tree Tree;
typedef struct node Node;
typedef struct edge Edge;
typedef struct graph Graph;

typedef int (*func)(); // general call-back function type

// general lst
struct lst{
	// empty head, general data type list
	
	// attributes
	void * pdata; // use for any data type
	Lst * prev;
	Lst * next;
	
	// methods
	func print, free;

	int (*len)(Lst * self);
	int (*push)(Lst * self, void * pdata);
	void * (*pop)(Lst * self);
	void * (*find)(Lst * self, void * pdata);
	int (*del)(Lst * self, void * pdata);
	Lst * (*at)(Lst * self,int idx);
};

int init_Lst(Lst ** self);

// B-tree
struct tree{
	// attributes
	int id;
	int rank;
	Lst * members;
	void * pdata;
	
	Tree * l; // left sub tree
	Tree * r; // right sub tree
	Tree * p; // parent
	
	// methodes
	func print, free;
	int (*fprint)(Tree *,char * fname);
};
int init_Tree(Tree ** self);

// Network

int init_Node(Node ** self);
struct node{
	int id;
	char * label;
	double data; // site energy, cm^-1 for pigments
	//method
	func print, free;
};

int init_Edge(Edge ** self);
struct edge{
	int id;
	Node * s;
	Node * t;
	double c;// capacity
	double f;// flow
	// methods
	func print, free;
};

int init_Graph(Graph ** self);
struct graph{
	Lst * nodes;
	Lst * edges;
	gsl_matrix * AM; // adjacency matrix
	gsl_vector * WM; // node weight
	// methods
	func print, free;
	int(*addEdge)(Graph * self, Node * s, Node * t);
	Node * (*getNode)(Graph * self, int id);
	Edge * (*getEdge)(Graph * self, int id);
};

