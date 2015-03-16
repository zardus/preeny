#include <ini_config.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include "logging.h"

struct collection_item *preeny_patch_load(char *conf_file)
{
	struct collection_item *diff_conf_errors = NULL;
	struct collection_item *diff_conf = NULL;
	int r;

	preeny_debug("loading config file %s\n", conf_file);

	r = config_from_file("preeny_patch", conf_file, &diff_conf, 0, &diff_conf_errors);
	if (r != 0)
	{
		if (errno == 0)
		{
			preeny_error("patch file %s contains errors\n", conf_file);
			if (preeny_error_on)
			{
				print_file_parsing_errors(stderr, diff_conf_errors);
				fprintf(stderr, "\n");
				free_ini_config_errors(diff_conf_errors);
			}
		}
		else perror("loading config file failed");
		return NULL;
	}
	else
	{
		preeny_debug("config file %s successfully loaded!\n", conf_file);
	}

	return diff_conf;
}

struct collection_item *preeny_patch_get_config_item(char *conf_file, char *section, struct collection_item *patch, char *name)
{
	struct collection_item *item = NULL;
	int error;

	error = get_config_item(section, name, patch, &item);
	if (!item || error != 0)
	{
		preeny_debug("couldn't get %s item from section %s in patchfile %s\n", name, section, conf_file);
		return NULL;
	}

	return item;
}

void *preeny_patch_get_pointer(char *conf_file, char *section, struct collection_item *patch, char *name)
{
	struct collection_item *item = NULL;
	int error;

	const char *ptr_str; 
	void *ptr;

	item = preeny_patch_get_config_item(conf_file, section, patch, name);
	if (!item) { preeny_error("error getting %s from section %s in patchfile %s\n", name, section, conf_file); return NULL; }

	ptr_str = get_const_string_config_value(item, &error);
	if (error != 0) { preeny_error("error converting %s from section %s in patchfile %s\n", name, section, conf_file); return NULL; }

	sscanf(ptr_str, "%p", &ptr);
	preeny_debug("retrieved %s: %p\n", name, ptr);
	return ptr;
}

void *preeny_patch_get_content(char *conf_file, char *section, struct collection_item *patch, int *content_length)
{
	struct collection_item *item = NULL;
	int error;

	void *content;

	item = preeny_patch_get_config_item(conf_file, section, patch, "content");
	if (!item) return NULL;

	content = get_bin_config_value(item, content_length, &error);
	if (error != 0) { preeny_error("error converting content from section %s in patchfile %s\n", section, conf_file); return NULL; }

	return content;
}

int preeny_patch_apply_patch(void *target, void *content, int length)
{
	char error_str[1024];
	int error;
	int pagesize = getpagesize();
	uintptr_t target_page = ((uintptr_t)target)/pagesize*pagesize;

	preeny_debug("mprotecting pages containing %d bytes at %p so that we can write the patch\n", length, target);
	error = mprotect((void *)target_page, length, PROT_READ | PROT_WRITE | PROT_EXEC);
	if (error != 0)
	{
		strerror_r(errno, error_str, 1024);
		preeny_error("error '%s' making pages containing %d bytes at %p writeable\n", error_str, length, target);
	}

	preeny_debug("writing %d bytes at %p\n", length, target);
	memcpy(target, content, length);

	preeny_debug("wrote %d bytes at %p\n", length, target);
}

int preeny_patch_apply_file(char *conf_file, struct collection_item *patch)
{
	int num_sections = 0;
	char **sections;
	char *section;
	int error = 0;
	int i;

	void *addr;
	void *content;
	int content_length = 0;

	sections = get_section_list(patch, &num_sections, &error);
	if (error > 0) { preeny_error("error getting section list from %s\n", conf_file); return -1; }

	for (i = 0; i < num_sections; i++)
	{
		section = sections[i];
		preeny_debug("apply patches for section %s in file %s\n", section, conf_file);

		addr = preeny_patch_get_pointer(conf_file, section, patch, "address");
		if (addr == NULL)
		{
			preeny_error("got NULL target for section %s from %s\n", section, conf_file);
			return -1;
		}

		content = preeny_patch_get_content(conf_file, section, patch, &content_length);
		if (content == NULL)
		{
			preeny_error("got NULL content for section %s from %s\n", section, conf_file);
			return -1;
		}

		preeny_info("section %s in file %s specifies %d-byte patch at %p\n", section, conf_file, content_length, addr);

		error = preeny_patch_apply_patch(addr, content, content_length);
		free(content);
		if (error > 0) { preeny_error("error applying patch section %s from %s\n", section, conf_file); return -1; }
	}
}

__attribute__((constructor)) void preeny_patch_program()
{
	char *patchfile = getenv("PATCH");
	struct collection_item *p;

	if (patchfile)
	{
		p = preeny_patch_load(patchfile);
		if (p == NULL) exit(137);

		preeny_patch_apply_file(patchfile, p);
	}

	preeny_debug("done patching!\n");
}
