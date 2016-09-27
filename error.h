#ifndef ERROR_H
#define ERROR_H
/*
	This function takes GString *, starting index, ending index,
	length of string, and function name, and checks bounds. 
	If invalid, print to stderr and exit with error code IND_OUT_BOUNDS.
*/
extern int bound_check(GString *g, int start, int end, int length, const char *func);
/*
	Checks if any exceptions, as defined in gstrlib.h, occur
*/

extern EXCEPTION err_check(GString *g, const char *func);

#endif