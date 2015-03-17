#include <stdlib.h>
#include "logging.h"

int rand(void)
{
	char *r_str = getenv("RAND");
	int r;

	if (r_str) r = atoi(r_str);
	else r = 42;

	preeny_debug("random changed to %d\n", r);
	return r;
}

int rand_r(unsigned int *a)
{
	return rand();
}

long random()
{
	return rand();
}
