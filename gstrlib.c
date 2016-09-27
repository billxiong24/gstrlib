#include "gstrlib.h"
#include "aux.h"
#include "error.h"

/*
	Internal struct that GString.internal points to.
	Contains contents of the actual string, as well as its length.
	This is done to hide struct from user, so that user cannot access it.
*/
typedef struct{
	char *builder;
	int length;
}GStr_Builder;



/*------------------ERROR CHECKING FUNCTIONS-----------------*/

int bound_check(GString *g, int start, int end, int length, const char *func){
	GStr_Builder *b = (GStr_Builder *) g->internal;
	/*In case user enters something past boundaries*/
	if(start < 0 || end < 0 || start >= length || end > length || start > end){
		fprintf(stderr, "Error: Index out of bounds in function %s\n", func);		
		exit(IND_OUT_BOUNDS);
	}

	return NO_ERROR;
}

/*Checks for null pointers, exits if found*/
EXCEPTION err_check(GString *g, const char *func){
	if(!g){
		fprintf(stderr, "Error: NULL POINTER EXCEPTION in function %s.\n", func);
		exit(NULL_PTR);
	}

	GStr_Builder *b = (GStr_Builder *) g->internal;
	
	if(!b){
		fprintf(stderr, "Error: NULL POINTER EXCEPTION in function %s.\n", func);
		exit(NULL_PTR);
	}

	return NO_ERROR;

}

/*--------------------"CONSTRUCTOR"----------------------*/

GString *new_GString(char * str){
	GString *gstring = malloc(sizeof(*gstring));
	gstring->internal = malloc(sizeof(GStr_Builder));
	/*cast void pointer to private struct*/
	GStr_Builder *b = (GStr_Builder *) gstring->internal;
	if(str == NULL){
		b->builder = NULL;
		b->length = 0;
	}
	else{
		b->length = len(str);
		b->builder = malloc(b->length * sizeof(char) + 1);
		copy(b->builder, str);
	}
	
	return gstring;
}

/*--------------------APPEND FUNCTIONS-------------------*/

GString *append_str(GString *g, char *str){
	err_check(g, __func__);
	GStr_Builder *b = (GStr_Builder *) g->internal;
	/*reallocate enough memory for the 2 strings*/
	b->builder = realloc(b->builder, sizeof(char) * (b->length + len(str)) + 1);	
	/*start from end of the first string and copy*/
	copy(b->builder + b->length, str);

	b->length += len(str);
	/*null terminator*/
	*(b->builder + b->length) = '\0';

	return g;
}

GString *append_char(GString *g, char c){
	err_check(g, __func__);
	GStr_Builder *b = (GStr_Builder *) g->internal;

	/*make room for one more char*/
	b->length++;
	/*add one byte for null terminator*/
	b->builder = realloc(b->builder, sizeof(char) * (b->length + 1 + 1));
	*(b->builder + b->length - 1) = c;
	/*null terminator*/
	*(b->builder + b->length) = '\0';
	return g;
}

GString *append_double(GString *g, double val){
	err_check(g, __func__);
	GStr_Builder *b = (GStr_Builder *) g->internal;

	char hold[12 * 4];
	sprintf(hold, "%f", val);
	int temp = len(hold);
	/*+1 for null terminator*/
	b->builder = realloc(b->builder, sizeof(char) * (b->length + temp + 1));
	/*copy new string at the end*/
	copy(b->builder + b->length, hold);
	/*update length*/
	b->length += temp;
	*(b->builder + b->length) = '\0';
	return g;
}

// /*Literally the same thing as append_double*/
GString *append_float(GString *g, float val){
	err_check(g, __func__);
	GStr_Builder *b = (GStr_Builder *) g->internal;
	
	char hold[12 * 4];
	sprintf(hold, "%f", val);
	int temp = len(hold);

	b->builder = realloc(b->builder, sizeof(char) * (b->length + temp + 1));
	copy(b->builder + b->length, hold);
	/*update length*/
	b->length += temp;
	*(b->builder + b->length) = '\0';
	return g;
}

/*
	Appending numbers is the same...I wonder if there is easier
	way to do this...
*/
GString *append_int(GString *g, int val){
	err_check(g, __func__);
	GStr_Builder *b = (GStr_Builder *) g->internal;

	char hold[12 * 4];
	sprintf(hold, "%d", val);
	int temp = len(hold);

	b->builder = realloc(b->builder, sizeof(char) * (b->length + temp + 1));
	copy(b->builder + b->length, hold);
	
	/*update length*/
	b->length += temp;
	/*null terminator*/
	*(b->builder + b->length) = '\0';
	return g;
}

/*----------------------ACCESS FUNCTIONS---------------------*/

char get(GString *g, int index){
	err_check(g, __func__);
	GStr_Builder *b = (GStr_Builder *) g->internal;
	if(index < 0)
		index += b->length;

	bound_check(g, index, index, b->length, __func__);
		return '\0';

	return *(b->builder + index);
}

int length(GString *g){
	err_check(g, __func__);
	GStr_Builder *b = (GStr_Builder *) g->internal;

	return b->length;
}

char *substring(GString *g, int start, int end){
	/*standard error checking*/
	err_check(g, __func__);
	GStr_Builder *b = (GStr_Builder *) g->internal;

	/*Handle negative indices like Python*/
	if(start < 0)
		start += b->length;
	if(end < 0)
		end += b->length;

	if(start >= end)
		return "";

	bound_check(g, start, end, b->length, __func__);
	char *fill = malloc(sizeof(char) * (end - start + 1));
	int i;
	/*Fill in temp, excluding the end index*/
	for(i = start; i < end; i++)
		fill[i - start] = b->builder[i];
	
	fill[end-start] = '\0';
	return fill;
}

char *reverse_substring(GString *g, int start, int end){
	/*standard error checking*/
	err_check(g, __func__);
	GStr_Builder *b = (GStr_Builder *) g->internal;

	/*Handle negative indices like Python*/
	if(start < 0)
		start += b->length;
	if(end < 0)
		end += b->length;

	if(start >= end)
		return "";

	bound_check(g, start, end, b->length, __func__);
	char *fill = malloc(sizeof(char) * (end - start + 1));
	int i;
	/*Fill in temp, excluding the end index*/
	for(i = start; i < end; i++)
		fill[end - i - 1] = b->builder[i];
	
	fill[end-start] = '\0';

	return fill;
}

int *find(GString *g, char *str, int n){
	err_check(g, __func__);
	GStr_Builder *b = (GStr_Builder *) g->internal;
	
	if(len(str) > b->length)
		return NULL;

	int i;
	int *arr = malloc(sizeof(int) * n);

	for(i = 0; i < n; i++)
		*(arr + i) = -1;

	char *src = b->builder;
	char *dest = str;
	register int j, k;
	int index = 0;
	for(i = 0; i < b->length; i++){
		/*as long as equal, continue*/
		for(j = i, k = 0; *src && *dest && src[j] == *dest;j++, k++, *dest++)
			;

		/*if dest == NULL, then we have found a match*/
		if(!*dest)
			arr[index++] = i;
		/*reset dest to beginning of str*/
		dest-=k;
	}
	return arr;
}

int index_of(GString *g, char *str){
	err_check(g, __func__);
	GStr_Builder *b = (GStr_Builder *) g->internal;
	if(len(str) > b->length)
		return -1;

	int i;
	char *src = b->builder;
	char *dest = str;
	register int j, k;
	for(i = 0; i < b->length; i++){
		/*as long as equal, continue*/
		for(j = i, k = 0; *src && *dest && src[j] == *dest;j++, k++, *dest++)
			;

		/*if dest == NULL, then we have found a match*/
		if(!*dest)
			return i;
		/*reset dest to beginning of str*/
		dest-=k;
	}
	return -1;
}

int index_from(GString *g, char *str, int index){
	err_check(g, __func__);
	GStr_Builder *b = (GStr_Builder *) g->internal;
	if(index < 0)
		index += b->length;

	bound_check(g, index, index, b->length, __func__);

	if(len(str) > (b->length-index))
		return IND_OUT_BOUNDS;

	char *src = b->builder;
	char *dest = str;
	int i, j, k;

	for(i = index; i < b->length; i++){
		for(j = i, k = 0; *src && *dest && src[j] == *dest; dest++, j++, k++);
		
		if(!*dest)
			return i;
		/*reset pointer to beginning of string*/
		dest-=k;
	}
	return -1;
}

int last_index_of(GString *g, char *str){
	err_check(g, __func__);
	GStr_Builder *b = (GStr_Builder *) g->internal;

	if(len(str) > b->length)
		return IND_OUT_BOUNDS;

	int dest_len = len(str);
	
	char *src = b->builder;
	char *dest = str + dest_len - 1;
	int i, j, k;

	/*Go and match in reverse*/
	for(i = b->length - 1; i >= 0; i--){

		for(j = i, k = dest_len - 1; k >= 0 && j >= 0 && src[j] == *dest; *dest--, j--, k--)
			;
		
		if(k == -1)
			return j + 1;

		/*reset pointer to end of the string*/
		dest += (dest_len-1) - k;
	}

	return -1;
}

int last_index_from(GString *g, char *str, int index){
	err_check(g, __func__);
	GStr_Builder *b = (GStr_Builder *) g->internal;

	if(index < 0)
		index += b->length;

	bound_check(g, index, index, b->length, __func__);
	if(len(str) > (b->length - index))
		return IND_OUT_BOUNDS;

	int dest_len = len(str);

	char *src = b->builder;
	char *dest = str + dest_len - 1;
	
	int i, j, k;

	for(i = index; i >= 0; i--){
		for(j = i, k = dest_len-1;j >= 0 && k >= 0 && src[j] == *dest; *dest--, j--, k--)
			;

		if(k == -1)
			return j + 1;
		/*reset pointer to end of string*/
		dest += (dest_len - 1) - k;
	}
	return -1;
}

int occur(GString *g, char *str){
	err_check(g, __func__);
	GStr_Builder *b = (GStr_Builder *) g->internal;

	if(len(str) > b->length)
		return 0;

	int i, total = 0;

	char *src = b->builder;
	char *dest = str;
	register int j, k;
	for(i = 0; i < b->length; i++){
		/*as long as equal, continue*/
		for(j = i, k = 0; *src && *dest && src[j] == *dest;j++, k++, *dest++)
			;

		/*if dest == NULL, then we have found a match*/
		if(!*dest)
			++total;
		/*reset dest to beginning of str*/
		dest-=k;
	}
	return total;
}

/*---------------------MODIFIER FUNCTIONS--------------------*/

GString *replace(GString *g, int start, int end, char *str){
	err_check(g, __func__);
	GStr_Builder *b = (GStr_Builder *) g->internal;

	if(start < 0)
		start += b->length;
	if(end < 0)
		end += b->length;

	bound_check(g, start, end, b->length, __func__);

	if(start >= end)
		return g;

	int i;
	for(i = start; *str && i < end; i++){
		*(b->builder + i) = *str++;	
	}
	/*Word was shorter than interval, remove excess characters*/
	if(i < end)
		delete(g, i, end);
	/*If word was longer than interval, insert remaining characters*/
	if(*str)
		insert(g, str, end);

	return g;
}

GString *delete(GString *g, int start, int end){
	err_check(g, __func__);
	GStr_Builder *b = (GStr_Builder *) g->internal;

	if(start < 0)
		start += b->length;
	if(end < 0)
		end += b->length;

	bound_check(g, start, end, b->length, __func__);

	if(start >= end)
		return g;
	
	char *p1 = b->builder + start;
	char *p2 = b->builder + end;
	int i;
	/*Length will decrease by end-start characters*/
	b->length -= (end - start);
	for(i = start; *p2 ; i++){
		*p1++ = *p2++;
	}	
	/*terminate the string*/
	*p1 = '\0';
	
	return g;
}
GString *set_char(GString *g, char c, int index){
	err_check(g, __func__);
	GStr_Builder *b = (GStr_Builder *) g->internal;

	if(index < 0)
		index += b->length;

	bound_check(g, index, index, b->length, __func__);

	*(b->builder + index) = c;
	return g;
}

GString *remove_char_at(GString *g, int index){
	err_check(g, __func__);
	return delete(g, index, index + 1);
}

GString *remove_char(GString *g, char c, int occur){
	err_check(g, __func__);
	GStr_Builder *b = (GStr_Builder *) g->internal;
	bound_check(g, occur, occur, b->length, __func__);

	int track = 0;
	int i;

	for(i = 0; i < b->length && track < occur; i++){
		
		if(*(b->builder + i) == c)
			++track;
	}

	if(track == occur)
		return remove_char_at(g, i - 1);
	else //dont change anything
		return g;
}

GString *remove_num_char(GString *g, char c, int num){
	err_check(g, __func__);
	GStr_Builder *b = (GStr_Builder *) g->internal;
	bound_check(g, num, num, b->length, __func__);

	int i, j;
	for(i = 0, j = 0; i <  b->length && j < num; i++){

		if(*(b->builder + i) == c){
			remove_char_at(g, i--);
			++j;
		}
	}

}

GString *reverse(GString *g){
	err_check(g, __func__);
	GStr_Builder *b = (GStr_Builder *) g->internal;

	char *beg = b->builder;
	char *end = b->builder + b->length-1;
	char c;
	while(end >= beg){
		c = *beg;
		*beg++ = *end;
		*end-- = c;
	}

	return g;
}

GString *insert(GString *g, char *str, int index){
	err_check(g, __func__);
	GStr_Builder *b = (GStr_Builder *) g->internal;

	/*Handle negative indices*/
	if(index < 0)
		index += b->length;

	bound_check(g, index, index, b->length, __func__);	
	if(index >= b->length)
		return append_str(g, str);

	int l = len(str);
	char *temp = b->builder;
	/*make space for new string*/
	b->builder = malloc(sizeof(char) * b->length + l + 1);
	b->length += l;
	int i, j;
	
	/*copy the first half of the old string*/
	for(i = 0; i < index; i++)
		*(b->builder + i) = *(temp + i);

	/*copy the new string*/
	for(j = index; j < l + index; j++)
		*(b->builder + j) = *(str + j - index);
	
	/*copy the second half of the old string*/
	for(;j < b->length; j++)
		*(b->builder + j) = *(temp + j - l);
	
	/*Don't forget null terminator*/
	b->builder[j] = '\0';

	free(temp);
	return g;
}
// /*------------------MISCELLANEOUS FUNCTIONS----------------*/

char *toString(GString *g){
	err_check(g, __func__);
	GStr_Builder *b= (GStr_Builder *) g->internal;
	return b->builder;
}

GString *ltrim(GString *g){
	err_check(g, __func__);
	GStr_Builder *b = (GStr_Builder *) g->internal;

	/*find last occurrence of white space before*/
	int count = -1;
	while(b->builder[++count] == ' ')
		;

	replace(g, 0, count, "");
	return g;
}
GString *rtrim(GString *g){
	err_check(g, __func__);
	GStr_Builder *b = (GStr_Builder *) g->internal;

	char *end = b->builder + b->length - 1;
	int track = 0;

	/*Find the right most character that's not white space*/
	while(*(end-track) == ' '){
		--b->length;
		++track;
	}
	/*If white space was detected*/
	if(track > 0)
		*(b->builder + b->length) = '\0';

	return g;
}
GString *trim(GString *g){
	rtrim(g);
	ltrim(g);
	return g;
}
void print(GString *g){
	err_check(g, __func__);
	GStr_Builder *b = (GStr_Builder *) g->internal;
	printf("%s\nLength: %d\n", b->builder, b->length);
}

void for_each(GString *g, void *(*func)(char)){
	err_check(g, __func__);
	GStr_Builder *b = (GStr_Builder *) g->internal;
	char *trav = b->builder;

	while(*trav){
		func(*trav);
		*trav++;
	}
}

void free_GString(GString *g){
	err_check(g, __func__);
	GStr_Builder *b = (GStr_Builder *) g->internal;
	free(b->builder);
	free(b);
	free(g);
}