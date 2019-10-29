#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#ifdef __unix__
#include <sys/time.h>
#endif

int main()
{
	printf("Time: %d\n", time(0));
#ifdef __unix__
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    printf("Time: %ld.%ld\n", tv.tv_sec, tv.tv_usec);
    printf("TZ: -%d min, DST: %d\n", tz.tz_minuteswest, tz.tz_dsttime);
#endif
}   
