#define __NR_FORKCOUNTS 314

#include <stdio.h>
#include "getexeccounts.h"

int getExecCounts(int pid, int* pArray) {
	syscall(__NR_FORKCOUNTS, pid, pArray);
	return 0;
}
