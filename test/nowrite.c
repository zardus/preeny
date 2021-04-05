#include <assert.h>
#include <stdio.h>

int main()
{
	char buffer[1024];

	// readonly
	FILE *fRO = open("nowrite.c", O_RDONLY);
	assert(read(fRO, buffer, 1024) > 0);
	close(fRO);

	// writeonly
	FILE *fWO = open("nowrite.c", O_WRONLY);
	assert(write(fWO, buffer, 1024) == -1);
	close(fWO);

	// read/write
	FILE *fRW = open("nowrite.c", O_RDWR);
	assert(read(fRW, buffer, 1024) > 0);
	assert(write(fRW, buffer, 1024) == -1);
	close(fRW);

	// read + create
	FILE *fCR = open("nowrite.ccc", O_WRONLY|O_CREAT);
	assert(write(fCR, buffer, 1024) == -1);
	close(fCR);

	// tempfile
	FILE *fTM = open("nowrite.c", O_WRONLY|O_TMPFILE);
	assert(write(fTM, buffer, 1024) == -1);
	close(fTM);
}
