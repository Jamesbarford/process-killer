#ifndef INTERNET_PROCESS_H
#define INTERNET_PROCESS_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

typedef struct internet_process_t {
	char *cmd;
	pid_t pid;
	char *usr;
	char *device;
	char *size_off;
	int node;
	char *name;
} internet_process_t;


void ipt_create(internet_process_t *ipt, char *src);
void ipt_destroy(internet_process_t *ipt);
void ipt_print_headers();
void ipt_print_std(internet_process_t *ipt);
void ipt_print_json(internet_process_t *ipt);

#endif
