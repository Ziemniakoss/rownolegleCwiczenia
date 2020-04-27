#include <stdio.h>
#include "int_counter.h"

int main(int argc, char ** argv){
	struct counter_t c;
	printf("%d size %d maxIndex\n", c.size, c.maxIndex);
	counter_init(&c, 2);
	        printf("%d size %d maxIndex\n", c.size, c.maxIndex);

	counter_add(&c, 0);
        counter_add(&c, 0);
        counter_add(&c, 0);
        counter_add(&c, 1);
        counter_add(&c, 1);
        counter_add(&c, 2);
        counter_add(&c, 3);
        counter_add(&c, 4);
        counter_add(&c, 4);
        counter_add(&c, 5);

	for(int i = 0; i <= c.maxIndex; i++)
		printf("%d: %d razy\n", c.ints[i], c.occurances[i]);
	counter_free(&c);
	return 0;
}
