#include <stdio.h>
#include <unistd.h>
#include <time.h>

int test_sleep() {
    if(sleep(1) < 0 )   
    {
        return -1;
    }

    return 0;
}

int test_usleep() {
    if(usleep(1000*1000) < 0 )   
    {
        return -1;
    }

    return 0;
}

int test_nanosleep() {
    struct timespec tim, tim2;
    tim.tv_sec = 1;
    tim.tv_nsec = 500000000L;

    if(nanosleep(&tim , &tim2) < 0 )   
    {
        return -1;
    }

    return 0;
}

int main()
{
   test_sleep();
   test_usleep();
   test_nanosleep();

   return 0;
}