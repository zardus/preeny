#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    uid_t uid = getuid();
    printf("I am running as UID = %d\n", uid);
    uid_t euid = geteuid();
    printf("I am running as EUID = %d\n", euid);

    return 0;
}
