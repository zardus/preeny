#define _GNU_SOURCE

#include <stdlib.h>
#include <dlfcn.h>
#include "logging.h"
#include <string.h>

//
// originals
//
void *(*original_malloc)(size_t);
void (*original_free)(void *);
void *(*original_realloc)(void *, size_t);
void *(*original_memcpy)(void *, const void*, size_t);
__attribute__((constructor)) void preeny_desock_dup_orig()
{
	original_malloc = dlsym(RTLD_NEXT, "malloc");
	original_free = dlsym(RTLD_NEXT, "free");
	original_realloc = dlsym(RTLD_NEXT, "realloc");
	original_memcpy = dlsym(RTLD_NEXT, "memcpy");
}


/*
	The goal of this function is to catch bugs where people
    assume realloc woudn't move stuff around e.g.

	ptr = malloc(...)
	realloc(ptr,...) <- ignore return value
	<use ptr somehow> <- could be double free, use-after-free, etc

	This is attempted by always assuring that realloc returns
	a new pointer to a new memory area regardless of if the old
	chunk needed to be moved or not.
*/

void *realloc(void *ptr, size_t size)
{
	void *r = original_realloc(ptr, size);
	preeny_info("realloc(%p, %d) == %p\n", ptr, size, r);
	if (r != ptr) {
		preeny_info("return %p\n", r);
		return r;
	} else {
		if (r) {
			void *rm = malloc(size);
			preeny_info("malloc(%u) == %p\n", size, rm);

			memcpy(rm, r, size);
			preeny_info("memcpy(%p, %p, %u)\n", rm, r, size);

			original_free(r);
			preeny_info("free(%p)\n", r);

			preeny_info("return %p\n", rm);
			return rm;

		} else {
			preeny_info("return %p\n", r);
			return r;
		}
	}
}
