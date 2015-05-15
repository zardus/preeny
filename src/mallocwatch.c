#define _GNU_SOURCE

#include <stdlib.h>
#include <dlfcn.h>
#include "logging.h"

//
// originals
//
void *(*original_malloc)(size_t);
void (*original_free)(void *);
void *(*original_calloc)(size_t, size_t);
void *(*original_realloc)(void *, size_t);
__attribute__((constructor)) void preeny_desock_dup_orig()
{
	original_malloc = dlsym(RTLD_NEXT, "malloc");
	original_free = dlsym(RTLD_NEXT, "free");
	original_calloc = dlsym(RTLD_NEXT, "calloc");
	original_realloc = dlsym(RTLD_NEXT, "realloc");
}

void *malloc(size_t size)
{
	void *r = original_malloc(size);
	preeny_info("malloc(%d) == %p\n", size, r);
	return r;
}

void free(void *ptr)
{
	original_free(ptr);
	preeny_info("free(%p)\n", ptr);
}

void *calloc(size_t nmemb, size_t size)
{
	void *r = original_calloc(nmemb, size);
	preeny_info("calloc(%d, %d) == %p\n", nmemb, size, r);
	return r;
}

void *realloc(void *ptr, size_t size)
{
	void *r = original_realloc(ptr, size);
	preeny_info("realloc(%p, %d) == %p\n", ptr, size, r);
	return r;
}
