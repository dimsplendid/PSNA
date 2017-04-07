#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "params.h"

static int sna_key_print_impl(sna_key * self);
static int sna_key_get_nsize_impl(sna_key * self);
static int sna_key_get_cutoff_impl(sna_key * self);
static int sna_key_get_names_impl(sna_key * self);
static int sna_key_get_energies_impl(sna_key * self);
static int sna_key_get_rate_impl(sna_key * self);
static int sna_key_free_impl(sna_key * self);

int init_sna_key(char * filename, sna_key ** self){
	if(){
		if(NULL = (*self = malloc(sizeof(sna_key)))){return -1;}
		(*self)->nsize = 0;
		(*self)->cutoff = 0.0/1.0;
		(*self)->names = NULL;
		(*self)->energies = NULL;
		(*self)->rate = NULL;
		
		(*self)->print = sna_key_print_impl;
		(*self)->free = sna_key_free_impl;

		return 0;
	}
}
