#include "logging.h"

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
