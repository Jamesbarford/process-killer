#include "internet_process.h"
// COMMAND  PID  USER   FD   TYPE DEVICE SIZE/OFF NODE NAME
char *example = "python3 4338 jwmbe    3u  IPv4  66018      0t0  TCP *:8000 (LISTEN)\n";

#define CLEAR_BUF(x) (memset(x, '\0', BUFSIZ))

static void *emalloc(size_t size) {
	void *ptr;

	if ((ptr = (void *)malloc(size)) == NULL) {
		fprintf(stderr, "Out of memory\n");
		exit(EXIT_FAILURE);
	}

	return ptr;
}

static void ipt_assign(internet_process_t *ipt, char *buf, unsigned int position) {
	int i;
	char *ptr;

	if (position == 1 || position == 5) {
		i = atoi(buf);
	} else {
		ptr = (char *)emalloc(sizeof(char) * strlen(buf));
	}

	switch (position) {
		case 0: {
			ipt->cmd = ptr;
			strcpy(ipt->cmd, buf);
			break;
		}
		case 1: ipt->pid = i; break;
		case 2: {
			ipt->usr = ptr;
			strcpy(ipt->usr, buf);
			break;
		}
		case 3: {
			ipt->device = ptr;
			strcpy(ipt->device, buf);
			break;
		}
		case 4: {
			ipt->size_off = ptr;
			strcpy(ipt->size_off, buf);
			break;		
		}
		case 5: ipt->node = i; break;
		case 6: {
			ipt->name = ptr;
			strcpy(ipt->name, buf);
			break;
		}
		default:
			return;
	}
}

void ipt_create(internet_process_t *ipt, char *src) {
	char c;
	char buf[BUFSIZ];
	CLEAR_BUF(buf);
	unsigned int position = 0;
	unsigned int i = 0;
	unsigned int char_count = 0;

	while ((c = src[i++]) != '\0') {
		if (!isspace(c)) {
			buf[char_count++] = c;
		} else {
			if (char_count != 0) {
				ipt_assign(ipt, buf, position++);
			}
			char_count = 0;
			CLEAR_BUF(buf);
		}
	}
}

void ipt_destroy(internet_process_t *ipt) {
	if (ipt->cmd) free(ipt->cmd);
	if (ipt->usr) free(ipt->usr);
	if (ipt->device) free(ipt->device);
	if (ipt->name) free(ipt->name);
	if (ipt->size_off) free(ipt->size_off);
	ipt->pid = -1;
	ipt->node = -1;
}

void ipt_print_json(internet_process_t *ipt) {
	fprintf(stderr,
			"{\n"
			"  \"command\": \"%s\",\n"
			"  \"pid\": %d,\n"
			"  \"user\": \"%s\",\n"
			"  \"device\": \"%s\",\n"
			"  \"size/off\": \"%s\",\n"
			"  \"node\": %d,\n"
			"  \"name\": \"%s\"\n"
			"}",
			ipt->cmd,
			ipt->pid,
			ipt->usr,
			ipt->device,
			ipt->size_off,
			ipt->node,
			ipt->name
		);
}

