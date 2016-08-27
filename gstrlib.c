#include "gstrlib.h"
#include "aux.h"

/*
	Internal struct that GString.internal points to.
	Contains contents of the actual string, as well as its length.
*/
typedef struct{
	char *builder;
	int length;
}GStr_Builder;

int bound_check(GString *, int, int, int, char *);
EXCEPTION err_check(GString *, char *func);


/*------------------ERROR CHECKING FUNCTIONS-----------------*/

int bound_check(GString *g, int start, int end, int length, char *func){
	
	GStr_Builder *b = (GStr_Builder *) g->internal;
	/*In case user enters something past boundaries*/
	if(start < 0 || end < 0 || start >= length || end > length || start > end){
		fprintf(stderr, "Error: Index out of bounds in function %s\n", func);		
		return IND_OUT_BOUNDS;
	}

	return 1;
}

EXCEPTION err_check(GString *g, char *func){

	if(!g){
		fprintf(stderr, "Error: NULL POINTER EXCEPTION in function %s.\n", func);
		return NULL_PTR;
	}

	GStr_Builder *b = (GStr_Builder *) g->internal;
	
	if(!b){
		fprintf(stderr, "Error: NULL POINTER EXCEPTION in function %s.\n", func);
		return NULL_PTR;
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

	if(err_check(g, "append_str") == NULL_PTR)
		return NULL;

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

	if(err_check(g, "append_char") == NULL_PTR)
		return NULL;

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

	if(err_check(g, "append_double") == NULL_PTR)
		return NULL;
	
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

	if(err_check(g, "append_float") == NULL_PTR)
		return NULL;

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

	if(err_check(g, "append_int") == NULL_PTR)
		return NULL;

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

	if(err_check(g, "get") == NULL_PTR)
		return '\0';

	GStr_Builder *b = (GStr_Builder *) g->internal;


	if(index < 0)
		index += b->length;

	if(bound_check(g, index, index, b->length,  "get") == IND_OUT_BOUNDS)
		return '\0';

	return *(b->builder + index);
}

int length(GString *g){
	if(err_check(g, "length") == NULL_PTR)
		return NULL_PTR;

	GStr_Builder *b = (GStr_Builder *) g->internal;

	return b->length;
}

char *substring(GString *g, int start, int end){
	/*standard error checking*/
	if(err_check(g, "substring") == NULL_PTR)
		return NULL;

	GStr_Builder *b = (GStr_Builder *) g->internal;

	/*Handle negative indices like Python*/
	if(start < 0)
		start += b->length;
	if(end < 0)
		end += b->length;

	if(start >= end)
		return "";

	if(bound_check(g, start, end, b->length, "substring") == IND_OUT_BOUNDS)
		return NULL;
	
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
	if(err_check(g, "reverse_substring") == NULL_PTR)
		return NULL;

	GStr_Builder *b = (GStr_Builder *) g->internal;

	/*Handle negative indices like Python*/
	if(start < 0)
		start += b->length;
	if(end < 0)
		end += b->length;

	if(start >= end)
		return "";

	if(bound_check(g, start, end, b->length, "reverse_substring") == IND_OUT_BOUNDS)
		return NULL;

	char *fill = malloc(sizeof(char) * (end - start + 1));
	int i;
	/*Fill in temp, excluding the end index*/
	for(i = start; i < end; i++)
		fill[end - i - 1] = b->builder[i];
	
	fill[end-start] = '\0';

	return fill;
}

int *find(GString *g, char *str, int n){

	if(err_check(g, "find") == NULL_PTR)
		return NULL;

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

	if(err_check(g, "index_of") == NULL_PTR)
		return NULL_PTR;
	
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

	if(err_check(g, "index_from") == NULL_PTR)
		return NULL_PTR;

	GStr_Builder *b = (GStr_Builder *) g->internal;

	if(index < 0)
		index += b->length;

	if(bound_check(g, index, index, b->length,  "index_from") == IND_OUT_BOUNDS)
		return IND_OUT_BOUNDS;

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

	if(err_check(g, "last_index_of") == NULL_PTR)
		return NULL_PTR;

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

	if(err_check(g, "last_index_from") == NULL_PTR)
		return NULL_PTR;

	GStr_Builder *b = (GStr_Builder *) g->internal;


	if(index < 0)
		index += b->length;

	if(bound_check(g, index, index, b->length,  "last_index_from") == IND_OUT_BOUNDS)
		return IND_OUT_BOUNDS;	

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
	
	if(err_check(g, "occur") == NULL_PTR)
		return NULL_PTR;
	
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
	
	if(err_check(g, "replace") == NULL_PTR)
		return NULL;
	
	GStr_Builder *b = (GStr_Builder *) g->internal;

	if(start < 0)
		start += b->length;
	if(end < 0)
		end += b->length;

	if(bound_check(g, start, end, b->length, "replace") == IND_OUT_BOUNDS)
		return g;

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

	if(err_check(g, "delete") == NULL_PTR)
		return NULL;
	
	GStr_Builder *b = (GStr_Builder *) g->internal;

	if(start < 0)
		start += b->length;
	if(end < 0)
		end += b->length;

	if(bound_check(g, start, end, b->length, "delete") == IND_OUT_BOUNDS)
		return g;


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

	if(err_check(g, "set_char") == NULL_PTR)
		return NULL;
	
	GStr_Builder *b = (GStr_Builder *) g->internal;

	if(index < 0)
		index += b->length;

	if(bound_check(g, index, index, b->length,  "set_char") == IND_OUT_BOUNDS)
		return g;


	*(b->builder + index) = c;
	return g;
}

GString *remove_char_at(GString *g, int index){

	if(err_check(g, "remove_char_at") == NULL_PTR)
		return NULL;

	return delete(g, index, index + 1);
}

GString *remove_char(GString *g, char c, int occur){

	if(err_check(g, "remove_char") == NULL_PTR)
		return NULL;

	GStr_Builder *b = (GStr_Builder *) g->internal;
	
	if(bound_check(g, occur, occur, b->length, "remove_char") == IND_OUT_BOUNDS)
		return NULL;

	int track = 0;
	int i;
	/*
		This loop discovers the index of char to remove
		If string terminates before number of occurences
		is reached, remove the last occurence of char.
	*/
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

	if(err_check(g, "remove_num_char") == NULL_PTR)
		return NULL;

	GStr_Builder *b = (GStr_Builder *) g->internal;

	if(bound_check(g, num, num, b->length, "remove_num_char") == IND_OUT_BOUNDS)
		return NULL;
	int i, j;
	for(i = 0, j = 0; i <  b->length && j < num; i++){

		if(*(b->builder + i) == c){
			/*remove_char calls delete, which automatically updates length of string*/
			remove_char_at(g, i--);
			++j;
		}
	}

}

GString *reverse(GString *g){
	
	if(err_check(g, "reverse") == NULL_PTR)
		return NULL;

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
	
	if(err_check(g, "insert") == NULL_PTR)
		return NULL;

	GStr_Builder *b = (GStr_Builder *) g->internal;

	/*Handle negative indices*/
	if(index < 0)
		index += b->length;

	if(bound_check(g, index, index, b->length, "insert") == IND_OUT_BOUNDS)
		return NULL;		

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

	if(err_check(g, "toString") == NULL_PTR)
		return NULL;

	GStr_Builder *b= (GStr_Builder *) g->internal;
	return b->builder;
}

GString *ltrim(GString *g){
	
	if(err_check(g, "ltrim") == NULL_PTR)
		return NULL;

	GStr_Builder *b = (GStr_Builder *) g->internal;

	/*find last occurrence of white space before*/
	int count = -1;
	while(b->builder[++count] == ' ')
		;

	replace(g, 0, count, "");
	return g;
}
GString *rtrim(GString *g){
	
	if(err_check(g, "rtrim") == NULL_PTR)
		return NULL;

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

	if(err_check(g, "print") == NULL_PTR)
		return;

	GStr_Builder *b = (GStr_Builder *) g->internal;

	printf("%s\nLength: %d", b->builder, b->length);
}

void free_GString(GString *g){
	
	if(err_check(g, "free_GString") == NULL_PTR)
		return;

	GStr_Builder *b = (GStr_Builder *) g->internal;

	free(b->builder);
	free(b);
	free(g);
}