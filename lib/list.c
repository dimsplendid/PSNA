#include <stdio.h>
#include <stdlib.h>

#include "data_struct.h"

static int lst_free_impl(Lst * self);
static int lst_len_impl(Lst * self);
static void * lst_pop_impl(Lst * self);
static int lst_del_impl(Lst * self, void * pdata);
static Lst * lst_at_impl(Lst * self,int idx);

// default, use for int
static void * lst_find_int_impl(Lst * self, void * pdata);
static int lst_push_int_impl(Lst * self,void * pdata);

int init_Lst(Lst ** self){ // call-by-value
	// the pointer to pointer make sure that the object
	// is not the copy.
	if(NULL == (*self = malloc(sizeof(Lst)))){ return -1;};
	(*self)->pdata = NULL;
	(*self)->next = NULL;
	(*self)->pre = NULL;

	(*self)->len = lst_len_impl;
	(*self)->push = lst_push_int_impl;
	(*self)->pop = lst_pop_impl;
	(*self)->find = lst_find_int_impl;
	(*self)->at = lst_at_impl;
	(*self)->del = lst_del_impl;

	(*self)->print = NULL; // depend on data type
	(*self)->free = lst_free_impl;
	return 0;
}

static int lst_del_impl(Lst * self, void * pdata){
	Lst * delNode;
	while(NULL != (delNode = self->find(self,pdata))){
		delNode->pre->next = delNode->next;
		free(delNode->pdata);
		free(delNode);
	}
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


// default lst impl, using for data type is int
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
	// return the first data found in lst
	Lst * tmp = self;
	int data = *(int*)pdata;
	do{
		int find = *(int*)tmp->pdata;
		if(find == data){ return tmp;} 
		else{tmp = tmp->next;}
	}while(tmp->next != NULL);
	return NULL;
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
	return 0;
}


#ifdef MAIN
int main(void){
	Lst * l = NULL;
	init_Lst(&l);
	
	l->print = lst_print_int_impl;

	for(int i = 0;i < 10;i++){
		l->push(l,&i);
	}
	printf("length of l: %d\n",l->len(l));
	l->print(l);
	l->free(l);
	return 0;
}
#endif

