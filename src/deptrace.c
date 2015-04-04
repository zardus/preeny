#define _GNU_SOURCE 1
#include <stdio.h>
#include "logging.h"

long ptrace(int a, int b, int c, int d) {
	preeny_info("ptrace blocked!\n");
	return 0;
}
