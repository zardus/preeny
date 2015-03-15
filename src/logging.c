// This code is GPLed by Yan Shoshitaishvili

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

int debug_on = 0;
int info_on = 1;
int error_on = 1;

void preeny_debug(char *fmt, ...)
{
	if (!debug_on) return;

	printf("+++ ");
	va_list args;
	va_start(args,fmt);
	vprintf(fmt,args);
	va_end(args);

	fflush(stdout);
}

void preeny_info(char *fmt, ...)
{
	if (!info_on) return;

	printf("||| ");
	va_list args;
	va_start(args,fmt);
	vprintf(fmt,args);
	va_end(args);

	fflush(stdout);
}

void preeny_error(char *fmt, ...)
{
	if (!error_on) return;

	fprintf(stderr, "### ERROR: ");
	va_list args;
	va_start(args,fmt);
	vfprintf(stderr, fmt,args);
	va_end(args);

	fflush(stderr);
}
