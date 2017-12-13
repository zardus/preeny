#define _GNU_SOURCE

#include <stdlib.h>
#include <time.h>
#include "logging.h"

time_t time(time_t *res)
{
	char *time_str = getenv("TIME");
	time_t time = time_str ? atoi(time_str) : 0;
	if(res != NULL) {
		*res = time;
	}

	preeny_debug("time frozen at %d\n", time);
	return time;
}
