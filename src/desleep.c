#include <stdlib.h>
#include <stdio.h>
#include "logging.h"

unsigned int sleep(unsigned int seconds)
{
  printf("Just spared you %u seconds!\n", seconds);
  return 0;
}
