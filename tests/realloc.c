#include "stdio.h"
#include <stdlib.h>

/*
$ PREENY_INFO=1 LD_PRELOAD=../src/crazyrealloc.so ./realloc
--- realloc(0x1967010, 15) == 0x1967010
--- malloc(15) == 0x1967030
--- memcpy(0x1967030, 0x1967010, 15)
--- free(0x1967010)
--- return 0x1967030
*** Error in `./realloc': double free or corruption (fasttop): 0x0000000001967010 ***
Aborted
*/

int main()
{
	char *s = malloc((size_t)10);
	realloc(s, (size_t)15);
	free(s);
}
