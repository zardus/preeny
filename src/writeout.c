#define _GNU_SOURCE

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdio.h>
#include <poll.h>
#include "logging.h"

//
// originals
//
ssize_t (*original_write)(int, const void *, size_t);
__attribute__((constructor)) void preeny_writeout_orig()
{
	original_write = dlsym(RTLD_NEXT, "write");
}

ssize_t write(int fd, const void *buf, size_t size)
{
	if (fd == 0) fd = 1;
	return original_write(fd, buf, size);
}
