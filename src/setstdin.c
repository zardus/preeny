#define _GNU_SOURCE

#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <dlfcn.h>

/*#include <unistd.h>*/
#define STDIN_FILENO 0
/* This should be better taken from unistd.h, but if you include it, you will get
conflict with read function declaration. I do not know how to fix it */

int (*original_fgetc)( FILE *stream );
size_t (*original_read)(int fd, void *data, size_t size);
size_t (*original_fread)(void *buffer, size_t size, size_t count, FILE *stream);


char default_stdin_data[] = "Default setstdin value. Please set PREENY_STDIN or PREENY_STDIN_FILENAME environment variables to set your own value\n";
char *set_stdin_data = NULL;

__attribute__((constructor)) void preeny_setstdin_dup_orig()
{
	original_read = dlsym(RTLD_NEXT, "read");
	original_fgetc = dlsym(RTLD_NEXT, "fgetc");
	original_fread = dlsym(RTLD_NEXT, "fread");

	/* Trying to get new STDIN content from environment variable */
	set_stdin_data = getenv("PREENY_STDIN");

	/* Trying to det new STDIN content from */
	if (! set_stdin_data)
	{
		char *file_name = getenv("PREENY_STDIN_FILENAME");
		if (file_name)
		{
			FILE *f;
			size_t len;
			f = fopen(file_name,"r");
			if(f == NULL)
			{
				fprintf(stderr, "Error opening file '%s': %s\n", file_name, strerror(errno));
				exit(EXIT_FAILURE);
			}
			/* Getting file size */
			fseek(f, 0L, SEEK_END);
			len = ftell(f);
			fseek(f, 0L, SEEK_SET);
			set_stdin_data = malloc(len + 1);
			if(f == NULL)
			{
				fprintf(stderr, "Out of memory\n");
				exit(EXIT_FAILURE);
			}
			if (fread(set_stdin_data, 1, len, f) !=len )
			{
				fprintf(stderr, "Error reading file '%s'\n",file_name);
				exit(EXIT_FAILURE);
			}
			set_stdin_data[len] = '\0'; /* Make 0-terminated string out of it */
		}
	}
	/* If no data for stdin were provided use default sample data */
	if (! set_stdin_data)
		set_stdin_data = default_stdin_data;
}

int getchar()
{
	int res;
	if (set_stdin_data[0] == '\0')
		return EOF;
	res = set_stdin_data[0];
	set_stdin_data++;
	return res;
}

int fgetc( FILE *stream )
{
	if (fileno(stream) != STDIN_FILENO)
		return original_fgetc(stream);
	return getchar();
}

int getc( FILE *stream )
{
	return fgetc(stream);
}

size_t read(int fd, void *data, size_t size)
{
	int len;
	/* If read from non-stdin file descriptor, just use original function*/
	if (fd != STDIN_FILENO)
		return original_read(fd, data, size);
	/* If set_stdin_data points to the end of line, return nothing*/
	if (set_stdin_data[0] == '\0')
		return 0;
	len = strlen(set_stdin_data);
	/* If requested size is less then set_stdin_data length, copy requested data */
        /* to the output, and move set_stdin_data to unused data */
	if (strlen(set_stdin_data) > size)
	{
		memcpy(data, set_stdin_data, size);
		set_stdin_data += size;
		return size;
	}
	/* If size is bigger then set_stdin_data length, return what we have and */
	/* move set_stdin_data to the very end of the string */
	memcpy(data, set_stdin_data, len);
	set_stdin_data += len;
	return len;
}

size_t fread(void *buffer, size_t size, size_t count, FILE *stream)
{
	if (fileno(stream) != STDIN_FILENO)
		return original_fread(buffer, size, count, stream);
	return read(STDIN_FILENO, buffer, size * count);
}
