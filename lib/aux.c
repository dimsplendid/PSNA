#include <stdio.h>
#include "aux.h"
void gsl_matrix_print(gsl_matrix * m){
	for(size_t i = 0; i < m->size1; i++){
		for(size_t j = 0; j < m->size2; j++){
			printf("% 12.6f ", gsl_matrix_get(m,i,j));
		}
		printf("\n");
	}
}
