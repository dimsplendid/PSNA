#include <stdio.h>
#include <stdlib.h>

#include "data_struct.h"

#define METHOD(NAME) (*self)->NAME = lst_ ##  NAME ## _impl

static int lst_free_impl(Lst * self);
static int lst_len_impl(Lst * self);
static void * lst_pop_impl(Lst * self);
static int lst_del_impl(Lst * self, void * pdata);
static Lst * lst_at_impl(Lst * self,int idx);

// default, use for int
static void * lst_find_int_impl(Lst * self, void * pdata);
static int lst_push_int_impl(Lst * self,void * pdata);
static int lst_print_int_impl(Lst * self);

int init_Lst(Lst ** self){ // call-by-value
	// the pointer to pointer make sure that the object
	// is not the copy.
	if(NULL == (*self = malloc(sizeof(Lst)))){ return -1;};
	(*self)->pdata = NULL;
	(*self)->next = NULL;
	(*self)->prev = NULL;

	METHOD(len);
	METHOD(pop);
	METHOD(at);
	METHOD(del);

	// depend on data type methods
	// default using int
	// need modified(polymorphism) for
	// other data type
	(*self)->find = lst_find_int_impl;
	(*self)->push = lst_push_int_impl;
	(*self)->print = lst_print_int_impl; 
	
	(*self)->free = lst_free_impl;
	return 0;
}

static int lst_del_impl(Lst * self, void * pdata){
	Lst * delNode;
	while(NULL != (delNode = self->find(self,pdata))){
		delNode->prev->next = delNode->next;
		free(delNode->pdata);
		free(delNode);
	}
	return 0;
}

static Lst * lst_at_impl(Lst * self, int idx){
	// zero-based numbering
	if (idx >= self->len(self)){
		printf("lst[%d] out of  range\n",idx);
		exit(EXIT_FAILURE);
	}
	Lst * tmp = self->next;
	for(int i = 0; i < idx; i++){
		tmp = tmp->next;
	}
	return tmp;
}
static void * lst_pop_impl(Lst * self){
	Lst * tmp = self;
	while(tmp->next != NULL){
		tmp = tmp->next;
	}
	void * pdata = tmp->pdata;
	free(tmp);
	tmp = NULL;
	return pdata;
}

static int lst_free_impl(Lst * self){
	if(self->next != NULL){
		lst_free_impl(self->next);
	}
	free(self->pdata);
	free(self);
	return 0;
}


static int lst_print_int_impl(Lst * self){
	Lst * n = self->next;
	while(n != NULL){
		printf("%d ",*(int*)(n->pdata));
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
	return c-1;
}

static void * lst_find_int_impl(Lst * self, void * pdata){
	// FIX
	// return the first data found in lst
	Lst * tmp = self->next;
	int data = *(int*)pdata;
	while(tmp != NULL){
		if( data == *(int*)tmp->pdata){break;}
		tmp = tmp->next;
	}
	return tmp;
}

static int lst_push_int_impl(Lst * self, void * pdata){
	int * new_pdata = malloc(sizeof(int));
	*new_pdata = *(int*)pdata;
	Lst * new = NULL;
	init_Lst(&new);
	new->pdata = new_pdata;
	Lst * tmp = self;
	while(tmp->next != NULL){
		tmp = tmp->next;
	}
	tmp->next = new;
	new->prev = tmp;
	return 0;
}


#ifdef MAIN
int main(void){
	Lst * l = NULL;
	init_Lst(&l);	

	for(int i = 0;i < 10;i++){
		l->push(l,&i);
	}
	int del = 2;
	l->push(l,&del);
	l->del(l,&del);
	printf("length of l: %d\n",l->len(l));
	l->print(l);
	l->free(l);
	return 0;
}
#endif

