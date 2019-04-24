#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>

#include "logging.h"

#define HOOK_FN(ret, name, args...) typedef ret (* name##_t)(args); \
ret name(args) { \
	static name##_t o_##name; \
	if (!o_##name) \
		o_##name = (name##_t) dlsym(RTLD_NEXT, #name); \
	ret result = o_##name
#define HOOK_END(cond) if (result cond && !isatty(fileno(stdin))) { \
	preeny_info("EOF, exiting."); \
	exit(0); \
} else return result; }

int hook_fd;

HOOK_FN(char*, fgets, char *s, int size, FILE *stream)(s, size, stream);
HOOK_END(== NULL && fileno(stream) == hook_fd);

HOOK_FN(int, fgetc, FILE *stream)(stream);
HOOK_END(== EOF && fileno(stream) == hook_fd);

HOOK_FN(int, getc, FILE *stream)(stream);
HOOK_END(== EOF && fileno(stream) == hook_fd);

HOOK_FN(int, getchar, void)();
HOOK_END(== EOF);

HOOK_FN(char*, gets, char *s)(s);
HOOK_END(== NULL);

HOOK_FN(ssize_t, read, int fd, void *buf, size_t count)(fd, buf, count);
HOOK_END(<= 0 && fd == hook_fd);

char scanf_eof_on_malformed;
int handle_scanf_result(int result) {
	if (result == EOF) {
		preeny_info("EOF, exiting.");
		exit(0);
	}
	if (scanf_eof_on_malformed && !result) {
		preeny_info("Malformed scanf input, exiting.");
		exit(0);
	}
	return result;
}

typedef int (*vscanf_t)(const char *format, va_list ap);
vscanf_t o_vscanf;
int scanf(const char *format, ...) {
    va_list args;
    va_start(args, format);
	int result = o_vscanf(format, args);
    va_end(args);
	return handle_scanf_result(result);
}

typedef int (*vfscanf_t)(FILE *stream, const char *format, va_list ap);
vfscanf_t o_vfscanf;
int fscanf(FILE *stream, const char *format, ...) {
    va_list args;
    va_start(args, format);
	int result = o_vfscanf(stream, format, args);
    va_end(args);
    if (fileno(stream) == hook_fd)
		return handle_scanf_result(result);
	else
		return result;
}

__attribute__((constructor))
int main() {
	scanf_eof_on_malformed = getenv("SCANF_EOF_ON_MALFORMED") != NULL;

	char *fd_str = getenv("EOF_HOOK_FD");
	hook_fd = fd_str ? atoi(fd_str) : fileno(stdin);

	o_vscanf = dlsym(RTLD_NEXT, "vscanf");
	o_vfscanf = dlsym(RTLD_NEXT, "vfscanf");

	return 0;
}
