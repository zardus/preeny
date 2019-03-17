// compile: gcc -o deexec.so -fPIC -shared deexec.c -lseccomp
// run: LD_PRELOAD=./deexec.so ./prog
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>
#include <seccomp.h>

// save the original __libc_start_main
int (*original__libc_start_main)(
	int (*main)(int, char **, char **), int argc, char **argv,
	int (*init)(int, char **, char **), void (*fini)(void),
	void (*ldso_fini)(void));

__attribute__((constructor)) void preeny_deexec_dup_orig()
{
	original__libc_start_main = dlsym(RTLD_NEXT, "__libc_start_main");
}

void install_seccomp()
{
	// Init the filter
	scmp_filter_ctx ctx;
	ctx = seccomp_init(SCMP_ACT_ALLOW); // default action: allow
	
	// setup blacklist
	seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(execve), 0);
	seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(execveat), 0);
	
	// build and load the filter
	seccomp_load(ctx);
}

int __libc_start_main(
	int (*main)(int, char **, char **), int argc, char **argv,
	int (*init)(int, char **, char **), void (*fini)(void),
	void (*ldso_fini)(void))
{
	install_seccomp();
	return original__libc_start_main(main, argc, argv, init, fini, ldso_fini);
}

//int main()
//{
//	install_seccomp();
//}
