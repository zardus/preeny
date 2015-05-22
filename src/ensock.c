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
#include <fcntl.h>
#include <poll.h>
#include "logging.h"

//
// originals
//
__attribute__((constructor)) void preeny_socketize()
{
	char *port_str = getenv("PORT");
	int port = port_str ? atoi(port_str) : 1337;
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	int conn_fd;
	struct sockaddr_in serv_addr, client_addr;
	int client_len = sizeof(client_addr);

	if (fd < 0)
	{
		preeny_error("Socket error\n");
		exit(1);
	}

	int optval = 1;
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
	fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) & (~O_NONBLOCK));

	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port);
	if (bind(fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
		preeny_error("Unable to bind\n");
		exit(1);
	}

	listen(fd, 0);
	puts("LISTENED");
	conn_fd = accept(fd, (struct sockaddr *)&client_addr, &client_len);
	fcntl(conn_fd, F_SETFL, fcntl(conn_fd, F_GETFL, 0) & (~O_NONBLOCK));
	puts("ACCEPTED");
	if (conn_fd < 0)
	{
		preeny_error("Accept fail\n");
		exit(1);
	}
	close(fd);
	puts("CLOSED");

	dup2(conn_fd, 0);
	dup2(conn_fd, 1);
	dup2(conn_fd, 2);
	puts("DUPED");
}
