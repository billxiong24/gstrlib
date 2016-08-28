#include "gstrlib.h"
int test();
int main(){
	char *d = "Hello Worldo These are some os to test o!";
	GString *g = new_GString(d);
	puts(toString(g));
	remove_num_char(g, 'l', 4);
	remove_char(g, 'o', -4);
	puts(toString(g));
	printf("%d \n", length(g));
	free_GString(g);
<<<<<<< HEAD

=======
	test();
	return 0;
}

int test(){
	printf("%s\n", __func__);
>>>>>>> errors
	return 0;
}
