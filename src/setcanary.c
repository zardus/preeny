#include "logging.h"
#include <stdlib.h>
#include <stdint.h>

__attribute__((constructor)) int preeny_set_canary()
{
	char *new_canary_str = getenv("CANARY");
	if (!new_canary_str)
	{
		preeny_error("CANARY environment variable not specified. Aborting setcanary.\n");
		return 0;
	}
	uintptr_t new_canary = strtoull(new_canary_str, NULL, 0);
	preeny_debug("Overwriting canary with %#x...\n", new_canary);
	__asm__ __volatile__ (
		".intel_syntax noprefix;"
		"mov rax, %0;"
		"mov QWORD PTR fs:0x28, rax;"
		".att_syntax;"
		:
		: "r"(new_canary)
		: "%rax"
	);
	return 0;
}
