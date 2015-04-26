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
int (*original_close)(int);
int (*original_dup2)(int, int);
__attribute__((constructor)) void preeny_desock_dup_orig()
{
	original_close = dlsym(RTLD_NEXT, "close");
	original_dup2 = dlsym(RTLD_NEXT, "dup2");
}

int close(int sockfd)
{
	if (sockfd <= 2)
	{
		preeny_info("Disabling close on %d\n", sockfd);
		return 0;
	}
	else
	{
		original_close(sockfd);
	}
}

int dup2(int old, int new)
{
	if (new <= 2)
	{
		preeny_info("Disabling dup from %d to %d\n", old, new);
		return 0;
	}
	else
	{
		original_dup2(old, new);
	}
}

int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
	preeny_info("Emulating accept on %d\n", sockfd);
	return 0;
}

int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	preeny_info("Emulating bind on port %d\n", ntohs(((struct sockaddr_in*)addr)->sin_port));
	return 0;
}

int listen(int sockfd, int backlog)
{
	return 0;
}
