#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main()
{
	int i;

	srand(time(0));

	for (i = 0; i < 10; i++)
	{
		printf("%d\n", i);
		printf("rand() returned %d\n", rand());
		//printf("random() returned %ld\n", random());
	}
}
