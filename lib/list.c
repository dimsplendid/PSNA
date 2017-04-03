#include <stdio.h>
#include <stdlib.h>

#include "data_struct.h"

static int lst_insert_impl(Lst * self, Lst *);
static int lst_push_impl(Lst * self,int data);
static int lst_print_impl(Lst * self);
static int lst_len_impl(Lst * self);
static int lst_free_impl(Lst * self);

int init_lst(Lst ** self){ // call-by-value
	// the pointer to pointer make sure that the object
	// is not the copy.
	if(NULL == (*self = malloc(sizeof(Lst)))){ return -1;};
	(*self)->data = 0;
	(*self)->next = NULL;
	(*self)->insert = lst_insert_impl;
	(*self)->push = lst_push_impl;
	(*self)->print = lst_print_impl;
	(*self)->len = lst_len_impl;
	(*self)->free = lst_free_impl;
	return 0;
}

static int lst_insert_impl(Lst * self, Lst * new){
	Lst * n1 = self;
	Lst * n2 = new;
	init_lst(&n2);
	n2->next = n1->next;
	n1->next = n2;
	return 0;
}

static int lst_push_impl(Lst * self, int data){
	Lst * n = self;
	Lst * new = NULL;
	init_lst(&new);
	new->data = data;

	while(n->next != NULL){
		n->next = new;	
	}
	return 0;
}

static int lst_print_impl(Lst * self){
	Lst * n = self;
	while(n != NULL){
		printf("%d ",n->data);
		n = n->next;
	}
	printf("\n");
	return 0;
}

static int lst_len_impl(Lst * self){
	Lst * n = self;
	int c = 0;
	while(n != NULL){
		c++;
		n = n->next;
	}
	return c;
}

static int lst_free_impl(Lst * self){
	if(self->next != NULL){
		lst_free_impl(self->next);
	}
	free(self);
	return 0;
}

#ifdef MAIN
int main(void){
	Lst * l = NULL;
	init_lst(&l);
	l->data = 10;
	l->insert(l,100);
	printf("length of l: %d\n",l->len(l));
	l->print(l);
	l->free(l);
	return 0;
}
#endif

