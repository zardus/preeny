#define _GNU_SOURCE

#include <stdlib.h>
#include <dlfcn.h>
#include "logging.h"

void srand(unsigned int s)
{
	char *r_str = getenv("WANT");
	char *skip_str = getenv("SKIP");
	char *mod_str = getenv("MOD");
	char *seed_str = getenv("SEED");
	int seed;
	int skip;
	int mod;
	int r;
	int i;

	void (*original_srand)(unsigned int);
	original_srand = dlsym(RTLD_NEXT, "srand");

	if (seed_str) seed = atoi(seed_str);
	else if (!r_str || !mod_str) seed = 42;
	else
	{
		if (!skip_str) skip = 0;
		else skip = atoi(skip_str);

		mod = atoi(mod_str);
		r = atoi(r_str);

		seed = 0;
		while (1)
		{
			original_srand(seed);
			preeny_debug("trying seed %d\n", seed);
			for (i = 0; i < skip; i++) rand();
			preeny_debug("... skipped %d values\n", i);
			i = rand();
			preeny_debug("... got: %d %% %d == %d\n", i, mod, i%mod);
			if (i%mod == r) break;
			seed++;
		}

		preeny_info("brute-forced seed of %d\n", seed);
	}

	preeny_debug("setting seed of %d\n", seed);
	original_srand(seed);
	return;
}
