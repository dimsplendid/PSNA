/***************************************************
 * parser.h
 * 
 * By Yuan-Chung Cheng
 * 
 * Header file for using parser functions.
 *
 * modified by Wei-Hsiang Tseng
 * email: dimsplendid@gmail.com
 ***************************************************/

#pragma once

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
typedef struct parser_keyword_struct {
  char *name;
  parser_callback_function function;
  int status;
} parser_keyword;

/* structure to hold a list of keywords */
typedef struct parser_klist_struct {
  int size;
  char delim_str[PARSER_STRING_BUFFER_SIZE];
  char rem_str[PARSER_STRING_BUFFER_SIZE];
  parser_keyword * member;

	//methods
	parser_callback_function free, print, 
			keyword_add, next_darray, next_int,
			set_delim, set_rem, set_default_func,
			input, dispatch;
	char * (*next_token)();
	char * (*next_line)();
	double (*next_double)();
	long (*next_long)();

} parser_klist;

int init_parser_klist(parser_klist ** self,int);


