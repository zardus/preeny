#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main()
{
	srand(time(0));
	printf("rand() returned %d\n", rand());
	printf("random() returned %ld\n", random());
}
