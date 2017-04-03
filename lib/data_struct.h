/*
* file: data_struct.h
* header file of list.c graph.c tree.c
* my first oop in c
*/

#pragma once
#include<stdlib.h>

typedef struct lst Lst; // dynamic link list
typedef struct tree Tree;
typedef struct node Node;
typedef struct link Link;
typedef struct graph Grahp;

typedef int (*func)(); // general call-back function type

struct lst{
	// attributes
	int data;
	Lst * next;
	// methods
	func len, insert,push , print, free;
};

int init_lst(Lst ** self);

struct tree{
	// attributes
	int id;
	int rank;
	double max_flw;
	Lst * member;
	int size;

	Tree * left;
	Tree * right;
	Tree * parent;
	
	// methodes
	func update_members;
	func cluster;
	func reconstruct;
	func print, fprint;
	func free;
};

int init_Tree(Tree ** self);
