/*--------------------GSTRING LIBRARY-----------------------*/
/*-------------An easier way to use strings in C------------*/

#ifndef GSTRLIB_H
#define GSTRLIB_H

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
/*
	TODO: ( )ADD EXIT STATUSES 
		  ( )Write automatic garbage collector??
		  ( )Write better memory allocator??
		  (x)Make struct members "private"
		  (x)Add error handling via enums
		  (x)Add negative indices to:
		  	-all access functions except length, find, occur, index_of
		  	-all modifier functions except reverse, remove_num_char

*/

typedef enum {ALLOC_ERR = 4, NULL_PTR = 3, IND_OUT_BOUNDS = 2, NO_ERROR = 0} EXCEPTION;

/*
	GString.internal points to internal struct that handles
	the string operations. This is done to abstract the struct members
	from users so they can't interact directly with them.
*/
typedef struct{
	void *internal;
}GString;

/*--------------------"CONSTRUCTOR"-------------------------*/
GString *new_GString(char *);

/*--------------------APPEND FUNCTIONS (6/6)----------------------*/

/*
	TODO: Fix "invalid write of size 1" error (FIXED)
*/
extern GString *append_str(GString *, char *); 
extern GString *append_char(GString *, char); 
extern GString *append_double(GString *, double); 
extern GString *append_float(GString *, float); 
extern GString *append_int(GString *, int); 

/*--------------------ACCESS FUNCTONS (8/8)----------------------*/

/*returns char at index. Can do negative indices! (like Python)*/
extern char get(GString *, int index);
/*returns number of non null characters in GString *.builder*/
extern int length(GString *);
/*
	returns string representation of substring between 2 indices
 	TODO: implement true variable arguments
*/
extern char * substring(GString *, int, int);
/*
	return substring from right to left given left and right index
*/
extern char *reverse_substring(GString *, int left, int right);
/*find first n occurences of str, return array of indices (-1) if not found*/
extern int *find(GString *, char * str, int n);
/*index of first occurence of char *, -1 if not found*/
extern int index_of(GString *, char *);
/*index of first occurence of char * from index, -1 if not found*/
extern int index_from(GString *, char *, int index);
/*index of first occurence of char * from right, -1 if not found*/
extern int last_index_of(GString *, char *); 
/*index of last occurence of char * less than ind, -1 if not found*/
extern int last_index_from(GString *, char *, int ind);
/*Return the number of occurences of string*/
extern int occur(GString *, char *);

/*--------------------MODIFIER FUNCTIONS (7/7)-------------------*/

/*
	TODO: Fix memory leak in insert function (FIXED)
		  Optimize remove_num_char function
*/

/*replace string from start to end */
extern GString * replace(GString *, int start, int end, char *);
/*deletes substring between start and end*/
extern GString * delete(GString *, int start, int end);
/*replaces character at index*/
extern GString * set_char(GString *, char, int index);
/*remove char at index (calls the delete function)*/
extern GString * remove_char_at(GString *, int index);
/*
	removes the (num)th char from the string
	(i.e. if num == 5, remove 5th occurrence of char)
*/

GString *remove_char(GString *, char, int num);

/*removes num occurences of char*/
extern GString * remove_num_char(GString *, char, int num);
/*reverse GString * */
extern GString * reverse(GString *);
/*
	insert string starting from index
	
	TODO: Fix memory leak (FIXED)
	Errors: Conditional jump or move depends on uninitialized values
	(FIXED)

*/
extern GString * insert(GString *, char *, int index);


/*------------------MISCELLANEOUS FUNCTIONS (6/6)----------------*/

/*
	TODO: Optimize trim functions (currently O(n))
*/

/*return string contained in internal struct*/
extern char * toString(GString *);
/*trims white spaces from left*/
extern GString *ltrim(GString *);
/*trims white spaces from right*/
extern GString * rtrim(GString *);
/*trim both sides*/
extern GString *trim(GString *);
/*prints contents of GString (string + length)*/
extern void print(GString *);
/*free everything*/
extern void free_GString(GString *);

#endif