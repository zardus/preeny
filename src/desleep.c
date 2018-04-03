#include "logging.h"
#include <time.h>

unsigned int sleep(unsigned int seconds)
{
  preeny_debug("Just spared you %u seconds!\n", seconds);
  return 0;
}

unsigned int usleep(unsigned int microseconds)
{
  preeny_debug("Just spared you %u microseconds!\n", microseconds);
  return 0;
}

int nanosleep(const struct timespec *req, struct timespec *rem) {
  preeny_debug("Just spared you %u seconds and %u nanoseconds!\n", req->tv_sec, req->tv_nsec);
  return 0;
}
