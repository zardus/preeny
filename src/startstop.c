#define _GNU_SOURCE

#include <sys/types.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>


//
// stop the process when it starts
//
__attribute__((constructor)) void preeny_startstop()
{
	kill(getpid(), SIGSTOP);
}
