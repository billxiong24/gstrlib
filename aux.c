#include "aux.h"
/*-----------------AUXILLARY/HELPER FUNCTIONS---------------*/
int len(char *str){
	char *temp = str;
	int count = 0;
	while(*temp++)
		count++;
	return count;

}

char *copy(char *dest, const char *src){

	char *temp = dest;
	while(*dest++ = *src++);
	return temp;
}
