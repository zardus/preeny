#define _GNU_SOURCE

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dlfcn.h>

int (*original_open)(const char *pathname, int flags, ...);

__attribute__((constructor)) void preeny_nowrite()
{
	original_open = dlsym(RTLD_NEXT, "open");
}

int open(const char *pathname, int flags, ...) {
	// Strip write-related flags & force readonly flag
	flags &= ~(O_WRONLY|O_RDWR|O_CLOEXEC|O_CREAT|O_DIRECTORY|O_EXCL|O_NOCTTY|O_NOFOLLOW|O_TMPFILE|O_TRUNC);
	flags |= O_RDONLY;

	return original_open(pathname, flags);
}

