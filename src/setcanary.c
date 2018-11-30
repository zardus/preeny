#include "logging.h"
#include <stdlib.h>
#include <stdint.h>

#ifdef __x86_64__
#define TONUMBER     strtoull
#define INSN_LOAD    "mov rax, %0;"
#define INSN_WRITE   "mov QWORD PTR fs:0x28, rax;"
#define REG          "%rax"

#elif __i386__
#define TONUMBER     strtoul
#define INSN_LOAD    "mov eax, %0;"
#define INSN_WRITE   "mov DWORD PTR gs:0x14, eax;"
#define REG          "%eax"
#endif

__attribute__((constructor)) int preeny_set_canary()
{
	char *new_canary_str = getenv("CANARY");
	if (!new_canary_str)
	{
		preeny_error("CANARY environment variable not specified. Aborting setcanary.\n");
		return 0;
	}
	uintptr_t new_canary = TONUMBER(new_canary_str, NULL, 0);
	preeny_debug("Overwriting canary with %#x...\n", new_canary);
	__asm__ __volatile__ (
		".intel_syntax noprefix;"
		INSN_LOAD
		INSN_WRITE
		".att_syntax;"
		:
		: "r"(new_canary)
        : REG
	);
	return 0;
}
