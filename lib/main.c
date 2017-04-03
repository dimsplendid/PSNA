#include <stdio.h>
#include "tree.h"

int main(void){
	lst * l = NULL;
	init_lst(&l);
	l->data = 10;
	l->insert(l,100);
	printf("length of l: %d\n",l->len(l));
	l->print(l);
	l->free(l);
	return 0;
}

