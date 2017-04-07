/***************************************************
 * parser.h
 * 
 * By Yuan-Chung Cheng
 * 
 * Header file for using parser functions.
 *
 ***************************************************/

#ifndef _PARSER_H
#define _PARSER_H 1

/* program constants */
#define PARSER_FILE_BUFFER_SIZE 65536
#define PARSER_STRING_BUFFER_SIZE 256
#define PARSER_DEFAULT_DELIM_CHARS " \t;:,=\n"
#define PARSER_DEFAULT_REM_CHARS "#"

/* symbolic names for the status of keyword parsing */
#define PARSER_REQUIRED 0
#define PARSER_OPTIONAL 1
#define PARSER_MULTIPLE 2
#define PARSER_DONE 3

/* general call back function type */
typedef int (*parser_callback_function)();

/* general call back function type */
typedef int (*parser_default_function)(char *);

/* structure to hold a keyword */
struct parser_keyword_struct 
{
  char *name;
  parser_callback_function function;
  int status;
};

typedef struct parser_keyword_struct parser_keyword;

/* structure to hold a list of keywords */
struct parser_klist_struct 
{
  int size;
  char delim_str[PARSER_STRING_BUFFER_SIZE];
  char rem_str[PARSER_STRING_BUFFER_SIZE];
  parser_keyword *member;
};

typedef struct parser_klist_struct parser_klist;

/* functions */
extern parser_klist* parser_klist_alloc(int);
extern void parser_klist_free(parser_klist*);

extern int 
parser_keyword_add(parser_klist*, char *, int, parser_callback_function);

extern char *parser_next_token();
extern char *parser_next_line();
extern double parser_next_double();
extern int parser_next_darray(double *,int);
extern int parser_next_int();
extern long parser_next_long();

extern int parser_set_delim(parser_klist *, char *);
extern int parser_set_rem(parser_klist *, char *);
extern int parser_set_default_func(parser_default_function);

extern int parser_parse_input(FILE *,parser_klist *);

#endif /* parser.h */
