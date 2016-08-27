#include "gstrlib.h"

int main(){
	char *d = "Hello World!";
	GString *g = new_GString(d);
	puts(toString(g));
	append_str(g, "testing private reirection");
	append_str(g, "howdy");
	append_char(g, 'Y');
	append_double(g, 243235.253);
	append_float(g, 46509.43);
	append_str(g, "wt");
	append_int(g, 54);
	puts(toString(g));
	putchar(get(g, -15));
	puts("");
	printf("%d \n", length(g));
	char *sub = substring(g, 3, 8);
	char *s = reverse_substring(g, 3, 8);

	int *c = find(g, "re", 2);
	int i;
	for(i = 0; i < 2; i++)
		printf("%d \n", c[i]);

	puts(sub);
	puts(s);
	printf("%d \n", index_of(g, "re"));
	printf("%d \n", index_from(g, "how", 30));
	printf("%d \n", last_index_of(g, "d"));
	printf("%d \n", last_index_from(g, "d", 25));
	printf("%d \n", occur(g, "re"));
	replace(g, 4, 8, "TESTING REPLACE");
	puts(toString(g));
	delete(g, 4, 8);
	puts(toString(g));
	set_char(g, 'X', 34);
	puts(toString(g));
	insert(g, "INSERITNG HERE", 12);
	puts(toString(g));
	reverse(g);
	puts(toString(g));
	insert(g, "      ", 0);
	puts(toString(g));
	ltrim(g);
	puts(toString(g));

	free(c);
	free(s);
	free(sub);
	free_GString(g);
	return 0;
}
