#define _GNU_SOURCE

#include <stdlib.h>
#include <time.h>
#ifdef __unix__
#include <sys/time.h>
#endif

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

#ifdef __unix__
int gettimeofday(struct timeval *tv, struct timezone *tz)
{
	
	char *sec_str = getenv("TV_SEC");
	char *usec_str = getenv("TV_USEC");
	tv->tv_sec = sec_str ? atoi(sec_str) : 0;
	tv->tv_usec = usec_str ? atoi(usec_str) : 0;

	preeny_debug("gettimeofday frozen at %ld.%d\n", tv->tv_sec, tv->tv_usec);
	
	
	if (tz != NULL) {
		char *minwest_str = getenv("TZ_MINWEST");
		char *dst_str = getenv("TZ_DSTTIME");
		tz->tz_minuteswest = minwest_str ? atoi(minwest_str) : 0;
		tz->tz_dsttime = dst_str ? atoi(dst_str) : 0;

		preeny_debug("gettimeofday tz frozen at -%d minm, DST: %d\n", tz->tz_minuteswest, tv->tv_usec, tz->tz_dsttime);
	}

	return 0;
}
#endif
