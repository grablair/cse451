#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../counts/getexeccounts.h"

void usage() {
	printf("Usage: execcnts <program>\n\tRuns the given program and shows it's exec/fork/clone counts.\n");
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
	if (argc < 2) usage();

	int child_pid;
	if (child_pid = fork()) {
		wait(child_pid);
		int pArray[] = {0, 0, 0, 0};
		getExecCounts(child_pid, pArray);
		printf("\n%d\tforks\n", pArray[0]);
		printf("%d\tvforks\n", pArray[1]);
		printf("%d\texecves\n", pArray[2]);
		printf("%d\tclones\n", pArray[3]);
	} else {
		// Launch new program
		char *file = argv[1];
		execvp(file, argv + 1);
	}

	return EXIT_SUCCESS;
}
