#pragma once

extern int debug_on;
extern int info_on;
extern int error_on;

void preeny_debug(char *, ...);
void preeny_info(char *, ...);
void preeny_error(char *, ...);
