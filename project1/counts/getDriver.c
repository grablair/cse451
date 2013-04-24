#include <stdio.h>
#include <stdlib.h>
#include "getexeccounts.h"

void usage() {
	printf("You must enter a pid to get the exec counts.\n");
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
	int pid;
	int pArray[] = {0, 0, 0, 0};

	if (argc < 2) usage();

	pid = atoi(argv[1]);

	if (!kill(pid, 0)) {
		printf("No process found with pid: %d\n", pid);
		exit(EXIT_FAILURE);
	}

	getExecCounts(pid, pArray);

	printf("pid %d:\n", pid);
	printf("\t%d\tfork\n", pArray[0]);
	printf("\t%d\tvfork\n", pArray[1]);
	printf("\t%d\texecve\n", pArray[2]);
	printf("\t%d\tclone\n", pArray[3]);

	return EXIT_SUCCESS;
}
