#include "logging.h"

unsigned int sleep(unsigned int seconds)
{
  preeny_debug("Just spared you %u seconds!\n", seconds);
  return 0;
}
