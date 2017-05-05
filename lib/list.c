#include <stdio.h>
#include <stdlib.h>

#include "data_struct.h"

static int lst_free_impl(Lst * self);
static int lst_len_impl(Lst * self);
static int lst_push_impl(Lst * self, Lst * newNode);
static void * lst_pop_impl(Lst * self);
static int lst_del_impl(Lst * self, Lst * delNode);
static Lst * lst_at_impl(Lst * self,int idx);

int init_Lst(Lst ** self){ // call-by-value
	// the pointer to pointer make sure that the object
	// is not the copy.
	if(NULL == (*self = malloc(sizeof(Lst)))){ return -1;};
	(*self)->pdata = NULL;
	(*self)->next = NULL;
	(*self)->pre = NULL;

	(*self)->len = lst_len_impl;
	(*self)->push = lst_push_impl;
	(*self)->pop = lst_pop_impl;
	(*self)->at = lst_at_impl;
	(*self)->del = lst_del_impl;

	(*self)->print = NULL; // depend on data type
	(*self)->free = lst_free_impl;
	return 0;
}

static int lst_del_impl(Lst * self, Lst * delNode){
	delNode->pre->next = delNode->next;
	free(delNode->pdata);
	free(delNode);
	return 0;
}

static Lst * lst_at_impl(Lst * self, int idx){
	Lst * tmp = self;
	int i = idx-1;
	while(i > 0){
		if(tmp->next == NULL){
			printf("lst[%d] out of range\n",idx);
			exit(EXIT_FAILURE);
		}
		else{
			tmp = tmp->next;
			i--;
		}
	}
	return tmp;
}

static int lst_push_impl(Lst * self, Lst * new){
	Lst * tmp = self;
	while(tmp->next != NULL){
		tmp = tmp->next;
	}
	tmp->next = new;
	return 0;
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

// use for data type is int
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

static int lst_free_impl(Lst * self){
	if(self->next != NULL){
		lst_free_impl(self->next);
	}
	free(self->pdata);
	free(self);
	return 0;
}

#ifdef MAIN
int main(void){
	Lst * l = NULL;
	init_Lst(&l);
	
	l->print = lst_print_int_impl;

	for(int i = 0;i < 10;i++){
		Lst * n = NULL;
		init_Lst(&n);
		int * tmp = malloc(sizeof(int));
		*tmp = i;
		n->pdata = (void*)tmp;
		l->push(l,n);
	}
	printf("length of l: %d\n",l->len(l));
	l->print(l);
	l->free(l);
	return 0;
}
#endif

