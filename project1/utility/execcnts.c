#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "getexeccounts.h"

void usage() {
	printf("Usage: execcnts <program>\n");
	printf("       Runs the given program and shows it's exec/fork/clone counts.\n");
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
	if (argc < 2) usage();

	int pArray[] = {0, 0, 0, 0};
	getExecCounts(getpid(), pArray);

	int child_pid;
	if (child_pid = fork()) {
		wait(child_pid);
		int pArray2[] = {-1, 0, 0, 0};	// Set to -1 because of utility's fork
		getExecCounts(getpid(), pArray2);
		printf("\n%d\tforks\n", pArray2[0] - pArray[0]);
		printf("%d\tvforks\n", pArray2[1] - pArray[1]);
		printf("%d\texecves\n", pArray2[2] - pArray[2]);
		printf("%d\tclones\n", pArray2[3] - pArray[3]);
	} else {
		// Launch new program
		char *file = argv[1];
		execvp(file, argv + 1);
	}

	return EXIT_SUCCESS;
}
