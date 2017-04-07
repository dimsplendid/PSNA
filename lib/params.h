#pragma once
#include "data_struct.h"
#include <gsl/gsl_matrix.h>

typedef struct simpleNA_key_struct {
	int nsize;
	double cut_off;
	char * names[];
	double energies[];
	gsl_matrix * rate;
	
	fun print;
	fun get_nsize;
	fun get_cutoff;
	fun get_names;
	fun get_energies;
	fun get_rate;
	fun free;

} sna_key;

int init_sna_key(char * filename, sna_key ** self);
