#include "gstrlib.h"

int main(){
	char *d = "Hello Worldo These are some os to test o!";
	GString *g = new_GString(d);
	puts(toString(g));
	remove_num_char(g, 'l', 4);
	remove_char(g, 'o', -12);
	puts(toString(g));
	printf("%d \n", length(g));
	free_GString(g);
	return 0;
}
