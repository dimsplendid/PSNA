#pragma once
#include <gsl/gsl_matrix.h>
typedef int (*func)();

typedef struct simpleNA_key_struct {
	int nsize;
	double cutoff;
	int * names;
	double * energies;
	gsl_matrix * rate;
	// methods
	func print;
	func free;
} sna_key;

int init_sna_key(sna_key ** self);

