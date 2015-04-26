#include <signal.h>
#include "logging.h"

int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact)
{
    preeny_info("sigaction on signal %d blocked\n", signum);
    return 0;
}
