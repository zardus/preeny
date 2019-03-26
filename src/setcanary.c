#include "logging.h"
#include <stdlib.h>
#include <stdint.h>

#ifdef __x86_64__
#define TONUMBER     strtoull
#define INSN_LOAD    "mov %0, %%rax;"
#define INSN_WRITE   "movq %%rax, %%fs:0x28;"
#define REG          "%rax"

#elif __i386__
#define TONUMBER     strtoul
#define INSN_LOAD    "mov %0, %%eax;"
#define INSN_WRITE   "movl %%eax, %%gs:0x14;"
#define REG          "%eax"

#elif __arm__
#define TONUMBER     strtoul
#define INSN_LOAD    "mov r4, %0;"
#define INSN_WRITE   "ldr r0, =__stack_chk_guard; mov r3, r0; ldr r1, =#0xfff; bic r0, r0, r1; mov r1, #0x1000; mov r2, #0x3; mov r7, #0x7d; svc 0; str r4, [r3]; ldr r0, =__stack_chk_guard; mov r3, r0; ldr r1, =#0xfff; bic r0, r0, r1; mov r1, #0x1000; mov r2, #0x1; mov r7, #0x7d; svc 0;"
#define REG          "r0", "r1", "r3", "r4", "r7"
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
		INSN_LOAD
		INSN_WRITE
		:
		: "r"(new_canary)
        : REG
	);
	return 0;
}
