#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <gsl/gsl_matrix.h>

#include "aux.h"
#include "parser.h"
#include "params.h"

// sna key method implement
static int sna_key_print_impl(sna_key * self);
static int sna_key_free_impl(sna_key * self);

int init_sna_key(sna_key ** self){
	if(NULL == (*self = malloc(sizeof(sna_key)))){
		fprintf(stderr,"error to malloc sna key: %s\n",strerror(errno));
		exit(errno);
	}
	// data
	(*self)->nsize = 0;
	(*self)->cutoff = 0.0/1.0;
	(*self)->names = NULL;
	(*self)->energies = NULL;
	(*self)->rate = NULL;
	
	// methods
	(*self)->print = sna_key_print_impl;
	(*self)->free = sna_key_free_impl;
	return 0;
}
static int sna_key_free_impl(sna_key * self){
	free(self->names);
	free(self->energies);
	gsl_matrix_free(self->rate);
	free(self);
	return 0;
}

static int sna_key_print_impl(sna_key * self){
	printf("\n");
	printf("NSIZE = %d\n",self->nsize);
	printf("\n");
	printf("Cut off = %f\n",self->cutoff);
	printf("\n");
	printf("Names: ");
	for(int i = 0; i < self->nsize; i++){
		printf("%d\t",self->names[i]);
	}
	printf("\n");
	for(int i = 0; i < self->nsize; i++){
		printf("%f\t",self->energies[i]);
	}
	printf("\n");
	printf("--------------------------------\n");
	printf("| Rate constant matrix (ps^-1) |\n");
	printf("--------------------------------\n");
	printf("\n");
	gsl_matrix_print(self->rate);
	printf("\n");
	return 0;
}


// global key
static sna_key * KEY;

// functions for parser

static int f_nsize(parser_klist * self){
	size_t n;
	KEY->nsize=self->next_int(self);
	if(KEY->nsize < 1){
		printf("Error: NSIZE must be >= 1.\n");
		exit(EXIT_FAILURE);
	}
#ifdef MAIN
	printf("read size... %d\n",KEY->nsize);
#endif
	n = KEY->nsize;
	KEY->names = calloc(n,sizeof(int));
	KEY->energies = calloc(n,sizeof(double));
	KEY->rate = gsl_matrix_alloc(n,n);
	return 0;
}

static int f_cutoff(parser_klist * self){
	KEY->cutoff = self->next_double(self);
#ifdef MAIN
	printf("read cut off... %f\n",KEY->cutoff);
#endif
	return 0;
}

static int f_node_name(parser_klist * self){
	for(int i = 0; i < KEY->nsize; i++){
		KEY->names[i] = self->next_int(self);
#ifdef MAIN
		printf("read names... %d\n",KEY->names[i]);
#endif
	}
	return 0;
}

static int f_node_energy(parser_klist * self){
	for(int i = 0; i < KEY->nsize; i++){
		KEY->energies[i] = self->next_double(self);
#ifdef MAIN
		printf("read energies... %f\n",KEY->energies[i]);
#endif
	}
	return 0;
}
/* aux function used to read in a matrix in MATRIX or ASSIGN
   format, YCC */
static void read_matrix(gsl_matrix *m, char *description){
  char *str;
  int n = KEY->nsize;
  if(n < 1) {
    printf("Error: NSIZE must be assigned before %s.\n",description);
    exit(EXIT_FAILURE);
  }
	// using parser funcitons
	parser_klist * parser;
	init_parser_klist(&parser,0);

  if(!(str=parser->next_token(parser))) {
    fprintf(stderr,"Error while reading %s!\n",description);
    exit(EXIT_FAILURE);
  }

  if(! strncasecmp(str,"MAT",3)) {
    /* matrix type input, read in all n*n elements */
    for(int i = 0; i < n; i++) {
      for(int j = 0; j < n; j++) {
	gsl_matrix_set(m,i,j,parser->next_double(parser));
      }
    }
    str=parser->next_token(parser);
    if(strncasecmp(str,"END",3)) {
      /* not end in a block ending "END" */
      fprintf(stderr,"Error while reading %s!\n",description);
      exit(EXIT_FAILURE);
    }
  } else if (! strncasecmp(str,"ASSI",4)) {
    /* assign type input, read in pair assigned elements:
       site1 site2 matrix_elements */

    gsl_matrix_set_zero(m);

    while(1) {
      str=parser->next_token(parser);
      if(! strncasecmp(str,"END",3)) {
	break;
      } else {
	int n1,n2;
	double tmp;
	n1=atoi(str);
	n2=parser->next_int(parser);
	tmp=parser->next_double(parser);
#ifdef MAIN
	printf("(%d,%d) -> %f\n",n1,n2,tmp);
#endif
	if((n1<1 || n2<1) || (n1>n || n2>n)) {
	  fprintf(stderr,"Error reading %s, site number (%d,%d) not in 1..%d!\n",
		  description,n1,n2,n);
	  exit(EXIT_FAILURE);
	}
	/* Note: index in keyword file starts from 1,
	   and the matrix has to be symmetric */
	gsl_matrix_set(m,n1-1,n2-1,tmp);
	gsl_matrix_set(m,n2-1,n1-1,tmp);
      }
    }
  } else {
    fprintf(stderr,"Error: unknown format type while reading %s!\n",description);
    exit(EXIT_FAILURE);
  }
}

int f_rate_constant(parser_klist * self){
	if(KEY->nsize < 1){
		printf("Error: NSIZE must be assigned before RATECONSTANT.\n");
		exit(EXIT_FAILURE);
	}
	read_matrix(KEY->rate,"RATECONSTANT");
	return 0;
}

/**********************************************
 *         parameters initialized             *
 **********************************************/
int params_init(char * filename, sna_key * key){
	KEY = key;
	// required keywords
	parser_klist * list;
	init_parser_klist(&list,300);
	list->keyword_add(list,"NSIZE",PARSER_REQUIRED,f_nsize);
	list->keyword_add(list,"CUTOFF",PARSER_REQUIRED,f_cutoff);
//	list->keyword_add(list,"CLUSTERCUTOFF",PARSER_REQUIRED,f_cluster_cutoff);
	list->keyword_add(list,"NODENAME",PARSER_REQUIRED,f_node_name);
	list->keyword_add(list,"NODEENERGY",PARSER_REQUIRED,f_node_energy);
	list->keyword_add(list,"RATECONSTANT",PARSER_REQUIRED,f_rate_constant);

	//read key file
	FILE * input_file = fopen(filename,"r");
	if(input_file == NULL){
		fprintf(stderr,"error while opening \"%s\" for reading: %s\n",
						filename,strerror(errno));
		exit(errno);
	}

	if(!list->input(list,input_file)){
		printf("Input file parsing error!\n");
		exit(EXIT_FAILURE);
	}
	fclose(input_file);
	list->free(list);
	key->print(key);
	return 0;
}

#ifdef MAIN
int main(void){
	sna_key * key = NULL;
	init_sna_key(&key);
	params_init("test.key",key);
	key->free(key);
	return 0;
}
#endif

