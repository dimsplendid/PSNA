/*
* implement the method of Tree
*/

#include <stdio.h>
#include <stdlib.h>

#include "data_struct.h"

static int update_members_impl(Tree * self);
static int cluster_impl( Tree * self);
static int reconstruct_impl( Tree * self,int option);
static int print_impl(Tree *self);
static int fprint_impl(Tree * self, FILE * f);
static int free_impl(Tree * self);

int init_tree(Tree ** self){ // call-by-value
	// the pointer to pointer make sure that the object
	// is not the copy.
	if(NULL == (*self = malloc(sizeof(Tree)))){ return -1;}
	(*self)->id = 0;
	(*self)->rank = 0;
	(*self)->max_flw = 1.0/0.0;
	(*self)->member = NULL;
	(*self)->size = 0;
	init_lst(&((*self)->member));
	(*self)->left = NULL;
	(*self)->right = NULL;
	(*self)->parent = NULL;
	
	(*self)->update_members = update_members_impl;
	(*self)->cluster = cluster_impl;
    (*self)->reconstruct = reconstruct_impl;
    (*self)->print = print_impl;
    (*self)->fprint = fprint_impl;
	(*self)->free = free_impl;

	return 0;
}

static void get_member(Tree * root, Lst * members){
	Lst * n = members;
	if(root != NULL){
		if((root->left == NULL) && (root->right == NULL)){
			if(n->data == -1){ n->data = root->member->data; }
			else{n->push(n,root->member->data); }
		}
		else{
			get_member(root->left,n);
			get_member(root->right,n);
		}
	}
}

static int update_members_impl(Tree * self){
	if((self->left != NULL) || (self->right != NULL)){
		self->member->free(self->member);
		self->member=NULL;
		init_lst(&(self->member));
		self->member->data=-1;
		get_member(self,self->member);
	}
	
	return 0;
}

static int cluster_impl( Tree * self){
	
	return 0;
}

static void reconstruct_0( Tree * root){
}

static int reconstruct_impl( Tree * self, int option){
	switch (option){
		case 0:
			reconstruct_0(self);
			break;

		default:
			printf("option %d hasn't been implement!\n",option);
	}
	return 0;
}

static int print_impl(Tree *self){
	if( self != NULL){
		printf("rank: %d Id: %d\n",self->rank,self->id);
		printf("Max flow: %.2f\n",self->max_flw);
		printf("members: ");
		self->update_members(self);
		self->member->print(self->member);
		printf("------------------------\n");
		print_impl(self->left);
		print_impl(self->right);
	}
	return 0;
}
static int fprint_impl(Tree * self, FILE * f){
	return 0;
}
static int free_impl(Tree * self){
	if(self->left != NULL){free_impl(self->left);}
	if(self->right != NULL){free_impl(self->right);}
	free(self);

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
		tree[i]->max_flw = (double)i;
		tree[i]->member->data = i;
	}
	tree[0]->left = tree[1];
	tree[0]->right = tree[2];
	
	tree[1]->parent = tree[0];
	tree[1]->rank = 0;
	tree[2]->parent = tree[0];
	tree[2]->rank = 0;
	
	// test implement method
	// tree->print
	tree[0]->print(tree[0]);

	return 0;
}
#endif
