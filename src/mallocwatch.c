#define _GNU_SOURCE

#include <stdlib.h>
#include <dlfcn.h>
#include "logging.h"


int malloc_hook_active = 1;
int free_hook_active = 1;
int calloc_hook_active = 1;
int realloc_hook_active = 1;

//
// originals
//
void *(*original_malloc)(size_t);
void (*original_free)(void *);
void *(*original_calloc)(size_t, size_t);
void *(*original_realloc)(void *, size_t);
__attribute__((constructor)) void preeny_mallocwatch_orig()
{
	original_malloc = dlsym(RTLD_NEXT, "malloc");
	original_free = dlsym(RTLD_NEXT, "free");
	original_calloc = dlsym(RTLD_NEXT, "calloc");
	original_realloc = dlsym(RTLD_NEXT, "realloc");
}

void *malloc(size_t size)
{
	void *r = original_malloc(size);
	if(malloc_hook_active) {
		malloc_hook_active = 0;
		preeny_info("malloc(%d) == %p\n", size, r);
		malloc_hook_active = 1;
	}
	return r;
}

void free(void *ptr)
{
	original_free(ptr);
	if(free_hook_active) {
		free_hook_active = 0;
		preeny_info("free(%p)\n", ptr);
		free_hook_active = 1;
	}
}

void *calloc(size_t nmemb, size_t size)
{
	void *r = original_calloc(nmemb, size);
	if(calloc_hook_active) {
		calloc_hook_active = 0;
		preeny_info("calloc(%d, %d) == %p\n", nmemb, size, r);
		calloc_hook_active = 1;
	}
	return r;
}

void *realloc(void *ptr, size_t size)
{
	void *r = original_realloc(ptr, size);
	if(realloc_hook_active) {
		realloc_hook_active = 0;
		preeny_info("realloc(%p, %d) == %p\n", ptr, size, r);
		realloc_hook_active = 1;
	}
	return r;
}
