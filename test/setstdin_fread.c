#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main() {
	char buffer[1000];
	int amount_read;
	int fd;
	int amount_to_read = 1;
	char separator[] = "|";

	fd = fileno(stdin);
	while (amount_read = fread(buffer, sizeof(char), amount_to_read, stdin))
	{
		if (amount_read == -1)
		{
			perror("error reading");
			return EXIT_FAILURE;
		}

		if (fwrite(buffer, sizeof(char), amount_read, stdout) == -1)
		{
			perror("error writing");
			return EXIT_FAILURE;
		}

		if (fwrite(separator, sizeof(char), strlen(separator), stdout) == -1)
		{
			perror("error writing");
			return EXIT_FAILURE;
		}

		amount_to_read++;
		if (amount_to_read > sizeof(buffer))
			 amount_to_read = sizeof(buffer);
	}
	return EXIT_SUCCESS;
}

