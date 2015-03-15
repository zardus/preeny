#include <unistd.h>
#include "logging.h"

pid_t fork(void) {
    preeny_info("fork blocked\n");
    return 0;
}
