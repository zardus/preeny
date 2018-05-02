#define _GNU_SOURCE

#include <stdlib.h>
#include "logging.h"

uid_t getuid(void)
{
	char *uid_str = getenv("FAKE_UID");
	uid_t uid = uid_str ? atoi(uid_str) : 0;

	preeny_debug("User ID set to %d\n", uid);
	return uid;
}

uid_t geteuid(void)
{
	char *euid_str = getenv("FAKE_EUID");
	uid_t euid = euid_str ? atoi(euid_str) : 0;

	preeny_debug("User ID set to %d\n", euid);
	return euid;
}
