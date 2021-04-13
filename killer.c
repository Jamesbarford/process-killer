/**
 * Interactive process killer mainly for killing things on localhost
 *
 * Heavily inspired by Kernighan & Pike The unix programming environment p.191
 *
 * */
#include <stdio.h>
#include <signal.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "internet_process.h"

FILE *efopen(char *filename, char *mode) {
	FILE *fp;
	
	if ((fp = fopen(filename, mode)) == NULL) {
		fprintf(stderr, "Failed to open '%s': %s", filename, strerror(errno));
		exit(EXIT_FAILURE);
	}

	return fp;
}

int strindex(char *str1, char *str2) {
	int i = 0, k = 0, j;

	for (; str1[i] != '\0'; ++i) {
		for (j = i; str2[k] != '\0' && str1[j] == str2[k]; ++j, ++k);
		if (k > 0 && str2[k] == '\0') {
			return i;
		}
	}

	return -1;
}

char ttyn() {
	char buf[BUFSIZ];
	static FILE *tty = NULL;
	
	if (tty == NULL)
		tty = efopen("/dev/tty", "r");
	for (;;) {	
		if (fgets(buf, BUFSIZ, tty) == NULL || buf[0] == 'q') {
			exit(EXIT_SUCCESS);
		} else if (buf[0] == '!') {
			system(buf + 1);
			printf("!\n");
		} else {
			return buf[0];
		}
	}
}

int main(int argc, char **argv) {
	char *progname = argv[0];
	// Maybe this should be fed in through argv
	char *ps = "lsof -i -P -n";

	FILE *fin;
	char buf[BUFSIZ];
	internet_process_t ipt;

	if ((fin = popen(ps, "r")) == NULL) {
		fprintf(stderr, "%s: can't run: '%s' -> %s\n", progname, ps, strerror(errno));
		exit(EXIT_FAILURE);
	}

	fgets(buf, sizeof(buf), fin);

	while (fgets(buf, sizeof(buf), fin) != NULL) {
		if (argc == 1 || strindex(buf, argv[1]) >= 0) {
			ipt_create(&ipt, buf);
			ipt_print_json(&ipt);

			fprintf(stderr, "Kill pid: %d (y/N)? ", ipt.pid);

			if (ttyn() == 'y') {
				kill(ipt.pid, SIGKILL);
			} 
			ipt_destroy(&ipt);
		}	
	}

	exit(EXIT_SUCCESS);
}
