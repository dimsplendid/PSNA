/*
* implement the method of Tree
*/

#include <stdio.h>
#include <stdlib.h>

#include "data_struct.h"

static int tree_fprint_impl(Tree * self,char * fname);
static int print_impl(Tree *self);
static int free_impl(Tree * self);
static int tree_member_print_impl(Lst * members);

int init_tree(Tree ** self){ // call-by-value
	// the pointer to pointer make sure that the object
	// is not the copy.
	if(NULL == (*self = malloc(sizeof(Tree)))){ return -1;}
	(*self)->id = 0;
	(*self)->rank = 0;
	(*self)->members = NULL;
	(*self)->pdata = NULL;
	init_Lst(&((*self)->members));
	(*self)->members->print = tree_member_print_impl;

	(*self)->print = print_impl;
	(*self)->fprint = tree_fprint_impl;
	(*self)->free = free_impl;

	return 0;
}
static int print_impl(Tree *self){
	if( self != NULL){
		printf("rank: %d Id: %d\n",self->rank,self->id);
		printf("data: %f\n",*(double*)self->pdata);
		printf("members: ");
		self->members->print(self->members);
		printf("------------------------\n");
		print_impl(self->l);
		print_impl(self->r);
	}
	return 0;
}
static int tree_fprint_impl(Tree * self, char * fname){
	return 0;
}
static int free_impl(Tree * self){
	if(self->l != NULL){free_impl(self->l);}
	if(self->r != NULL){free_impl(self->r);}
	free(self->members);
	free(self->pdata);
	free(self);

	return 0;
}

static int tree_member_print_impl(Lst * members){
	Lst * n = members->next;
	while(n != NULL){
		printf("%d ",*(int*)(n->pdata));
		n = n->next;
	}
	printf("\n");
	return 0;

}

#ifdef MAIN
int main(void){
	Tree * tree[3];
	// input data
	for (int i = 0; i< 3; i++){
		tree[i]=NULL;
		init_tree(&tree[i]);
		tree[i]->id = i;
		int * j = malloc(sizeof(int));
		*j = i;
		double * pdata = malloc(sizeof(double));
		* pdata = (double)i;
		tree[i]->pdata = (void*)pdata;
		Lst * n = NULL;
		init_Lst(&n);
		n->pdata = (void*)j;
		tree[i]->members->push(tree[i]->members,n);
	}
	tree[0]->l = tree[1];
	tree[0]->r = tree[2];
	
	tree[1]->p = tree[0];
	tree[1]->rank = 0;
	tree[2]->p = tree[0];
	tree[2]->rank = 0;
	
	// test implement method
	// tree->print
	tree[0]->print(tree[0]);

	return 0;
}
#endif
