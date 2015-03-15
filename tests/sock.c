#include <sys/socket.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>

int main()
{
	int s = socket(AF_INET, SOCK_STREAM, 0);
	char buf[1024];
	FILE *zomg = fdopen(s, "w");

	assert(send(s, "HI!\n", 4, 0) == 4);
	assert(recv(s, buf, 1024, 0) == 5);
	fprintf(zomg, "You wrote: %s\n", buf);
}
