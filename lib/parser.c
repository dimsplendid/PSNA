/***************************************************
 * parser.c
 * 
 * By Yuan-Chung Cheng <yccheng@mit.edu>
 *
 * General input file parser...
 *
 * modified by Tseng, Wei-Hsiang <dimsplendid@gmail.com>
 *
 ***************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "parser.h"

char * activated_key = NULL;

// methods forward declaration
static int parser_klist_free_impl(parser_klist * self);
// static int parser_klist_print_impl(parser_klist * self);

static int parser_klist_keyword_add_impl(parser_klist * self, char * str, int status, parser_callback_function func);
static char * parser_klist_next_token_impl(parser_klist * self);
static char * parser_klist_next_line_impl(void);
static double parser_klist_next_double_impl(parser_klist * self);
static int parser_klist_next_darray_impl(parser_klist * self, double * ,int);
static int parser_klist_next_int_impl(parser_klist * self);
static long parser_klist_next_long_impl(parser_klist * self);
static int parser_klist_set_delim_impl(parser_klist * self, char *);
static int parser_klist_set_rem_impl(parser_klist * self, char *);
static int parser_klist_set_default_func_impl(parser_klist * self,parser_default_function );
static int parser_klist_input_impl(parser_klist * self, FILE *);
static int parser_klist_dispatch_impl(parser_klist * self, char * key);
// parser key list initialize

int init_parser_klist(parser_klist ** self,int n){
  if (NULL == ((*self)=malloc(sizeof(parser_klist)))) {
    fprintf(stderr,"error in parser.c (parser_klist_alloc): %s \n", 
	    strerror(errno));
    exit(errno);
  }

  (*self)->size=0;
  strncpy((*self)->delim_str,PARSER_DEFAULT_DELIM_CHARS,PARSER_STRING_BUFFER_SIZE);
  strncpy((*self)->rem_str,PARSER_DEFAULT_REM_CHARS,PARSER_STRING_BUFFER_SIZE);

  if (NULL == ((*self)->member=malloc(n*sizeof(parser_keyword)))) {
    fprintf(stderr,"error in parser.c (parser_klist_alloc): %s \n",
	    strerror(errno));
    exit(errno);
  }
	
	// init methods
	(*self)->free = parser_klist_free_impl;
//	(*self)->print = parser_klist_print_impl;
	(*self)->print = NULL;
	(*self)->keyword_add = parser_klist_keyword_add_impl;
	(*self)->next_token = parser_klist_next_token_impl;
	(*self)->next_line = parser_klist_next_line_impl;
	(*self)->next_double = parser_klist_next_double_impl;
	(*self)->next_darray = parser_klist_next_darray_impl;
	(*self)->next_int = parser_klist_next_int_impl;
	(*self)->next_long = parser_klist_next_long_impl;
	(*self)->set_delim = parser_klist_set_delim_impl;
	(*self)->set_rem = parser_klist_set_rem_impl;
	(*self)->set_default_func = parser_klist_set_default_func_impl;
	(*self)->input = parser_klist_input_impl;
	(*self)->dispatch = parser_klist_dispatch_impl;

  return 0;
}

static int parser_klist_free_impl(parser_klist * self){
  free(self->member);
  free(self);
	return 0;
}

static int parser_klist_keyword_add_impl(parser_klist * self, char *str, 
		       int status, parser_callback_function func){
  int i;

  i= self->size;
  self->member[i].name=str;
  self->member[i].status=status;
  self->member[i].function=func;
  self->size++;
	return 0;
}

static int parser_klist_set_delim_impl(parser_klist * self, char * str){
  strncpy(self->delim_str,str,PARSER_STRING_BUFFER_SIZE);
	return 0;
}

static int parser_klist_set_rem_impl(parser_klist * self, char *str){
  strncpy(self->rem_str,str,PARSER_STRING_BUFFER_SIZE);
	return 0;
}

static int parser_default_func(char * str){
  printf("ERROR: Unknown keyword \"%s\" !\n",str);
  return 0;
}

// by defaut 
parser_default_function parser_f=parser_default_func;

static int parser_klist_set_default_func_impl(parser_klist * self,parser_default_function func){
  parser_f=func;
	return 0;
}

static int isinstr(char c,char *str){
  char *p;

  p=str;

  while(*p != '\0')
    if( c == *p++)
      return true;

  return false;
}


static char * parser_strtok(char *s,char *delim){
  static char *pstr=NULL;
  static char *p=NULL;

  if (s != NULL) {
    pstr=s;
  } else {
    pstr=p;
  }

  /* skip a delim sections to the beginning of the next token */
  while(isinstr(*pstr,delim)){
    pstr++;
	}
#ifdef DEBUG_PARSER
	printf("pstr: %p\n",pstr);
#endif
  if (*pstr == '\0')
    return NULL;

  /* put the ending NULL char */
  p=pstr;
  while(!isinstr(*p,delim))
    p++;

  *p='\0';

  p++;

#ifdef DEBUG_PARSER
  printf("STRTOK: %s\n",pstr);
#endif

  return (char *) pstr;
}

static int prefix_strcasecmp(const char *a, const char *b){
  int i;
  i=0;
  
  while(a[i] != '\0' && b[i] != '\0') {
    if(toupper(a[i]) != toupper(b[i]))
      return 1;
    i++;
  }

  return 0;
}

static int parser_klist_getline_impl(FILE *ifile, size_t buffersize, char *buffer, 
		   char *delim, char *rem){
  int len;
  
  buffer[0]='\\';
  buffer[1]='\0';
  len=1;

  while(len < buffersize && buffer[len-1] == '\\') {

    fgets(buffer+len-1,buffersize-len,ifile);
    
    /* rephase the string */

    /* strip off comment chars */
    int i = 0;
    while(buffer[i] != '\0' && !isinstr(buffer[i],rem) )
      i++;
    buffer[i]='\0';
    
    len=strlen(buffer);

    if(buffer[len-1] == '\n') {
      buffer[len-1]='\0';
      len--;
    }

    if(len == 0) { /* remaining part is an empty string */
      buffer[0]='\\';
      buffer[1]='\0';
      len=1;
    }

    if (feof(ifile)) {
      if(len == 1 && buffer[0]=='\\') {
	len=0;
	buffer[0]='\0';
      }
      return len;
    }
  }
  return len;
}

static char * parser_klist_next_token_impl(parser_klist * self){
  return parser_strtok(NULL,self->delim_str);
}

static char * parser_klist_next_line_impl(void){
  char *pstr;
  if (NULL == (pstr=parser_strtok(NULL,"\n"))) {
    fprintf(stderr,"Error while parsing keyword \"%s\"!\n",activated_key);
    exit(EXIT_FAILURE);
  }
  return pstr;
}

static double parser_klist_next_double_impl(parser_klist * self){
  char *pstr;
#ifdef DEBUG_PARSER
	printf("next double...\n");

#endif

  if (NULL == (pstr=parser_strtok(NULL,self->delim_str))) {
    fprintf(stderr,"Error while parsing keyword \"%s\"!\n",activated_key);
    exit(EXIT_FAILURE);
  }
  
  return atof(pstr);  
}

static int parser_klist_next_darray_impl(parser_klist * self,
                                     double *vec,int length){
  char *pstr;
  int i;

  for (i=0;i<length;i++) {
    if (NULL == (pstr=parser_strtok(NULL,self->delim_str))) {
      fprintf(stderr,"Error while parsing keyword \"%s\"!\n",activated_key);
      exit(EXIT_FAILURE);
    }
    vec[i]=atof(pstr);
  }
  return length;
}

static int parser_klist_next_int_impl(parser_klist * self){
  char *pstr;
  if (NULL == (pstr=parser_strtok(NULL,self->delim_str))) {
    fprintf(stderr,"Error while parsing keyword \"%s\"!\n",activated_key);
    exit(EXIT_FAILURE);
  }
  return atoi(pstr);  
}

static long parser_klist_next_long_impl(parser_klist * self){
  char *pstr;

  if (NULL == (pstr=parser_strtok(NULL,self->delim_str))) {
    fprintf(stderr,"Error while parsing keyword \"%s\"!\n",activated_key);
    exit(EXIT_FAILURE);
  }
  return atol(pstr);  
}

static int parser_klist_dispatch_impl(parser_klist * self, char * key){
  int wsize = self->size;
  
  for(int i=0;i < wsize;i++) {

#ifdef DEBUG_PARSER
    printf("key = \"%s\", name = \"%s\"\n",key,self->member[i].name);
#endif

    if(!prefix_strcasecmp(self->member[i].name,key)) {
      if(self->member[i].status==PARSER_DONE) {
	printf("ERROR: Mutiple definition of keyword \"%s\" !\n",key);
	return 0;
      }
      activated_key=self->member[i].name; /* set the activated keyword */
      self->member[i].function(self); /* run the call back function */
      if(self->member[i].status != PARSER_MULTIPLE)
	self->member[i].status=PARSER_DONE;
      return 1; /* sucess */
    }
  }
  return parser_f(key);
}

static int parser_klist_input_impl(parser_klist * self, FILE *ifile){
  char cbuffer[PARSER_FILE_BUFFER_SIZE];
  char *key=NULL;
  int wsize = self->size;
  int retvalue=1;
  int i,j;
  
  // activated_klist=w; /* set the activated keylist */

  /* put everything into cbuffer */
  j=0;
  while(0 != (i = parser_klist_getline_impl(ifile, PARSER_FILE_BUFFER_SIZE-j, cbuffer+j, 
			 self->delim_str, self->rem_str))) {
    j+=i;
    cbuffer[j]='\n'; /* space with a newline */
    j++;
    cbuffer[j]='\0';
  }

#ifdef DEBUG_PARSER
  printf("READ:\"\n%s\n\"\n",cbuffer);
#endif

  /* init the token pool */
  key=parser_strtok(cbuffer,self->delim_str);
  if( !(self->dispatch(self,key)))
    return 0;

  /* process all tokens */
  while((key = parser_klist_next_token_impl(self))) {

#ifdef DEBUG_PARSER
    printf("Processing \"%s\"\n",key);
#endif

    if( !(self->dispatch(self,key)))
      return 0;
  }
  
  /* check if we got al required parameters */
  for(int i = 0;i<wsize;i++){
    if(self->member[i].status == PARSER_REQUIRED) {
      printf("ERROR: REQUIRED keyword \"%s\" not found!\n",self->member[i].name);
      retvalue=0;
    }
  }
  return retvalue;
}

#ifdef MAIN

int f_cell(parser_klist * klist){
  double tok;

  tok= klist->next_double(klist);
  printf("cell = \"%lf\"\n",tok);
	return 0;
}

int f_unit(parser_klist * klist){
  char *tok;

  tok=klist->next_token(klist);
  printf("unit = \"%s\"\n",tok);
	return 0;
}

int f_par(parser_klist * klist){
  char *tok;

  tok = klist->next_token(klist);
  printf("par = \"%s\"\n",tok);
	return 0;
}

int f_title(parser_klist * klist){
  char *tok;

  tok=klist->next_line(klist);
  printf("title = \"%s\"\n",tok);
	return 0;
}


int main(void){
	// int num, i;
  parser_klist *list;
  
  init_parser_klist(&list,300);

  list->keyword_add(list,"Cell",PARSER_REQUIRED,f_cell);
  list->keyword_add(list,"Unit",PARSER_OPTIONAL,f_unit);
  list->keyword_add(list,"par",PARSER_OPTIONAL,f_par);
  list->keyword_add(list,"title",PARSER_OPTIONAL,f_title);

	printf("test_parser...\n");
  if(!list->input(list,stdin))
    printf("Input file parsing error!\n");

  list->free(list);
	return 0;
}

#endif
