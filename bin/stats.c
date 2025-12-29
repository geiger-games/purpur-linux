
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main() {
	FILE *f = fopen("/proc/meminfo", "r");
	if (!f) {
		return 27; // file fail
	}

	char totalMem[128] = {0};
	if (fgets(totalMem, 127, f)) {
		totalMem[strcspn(totalMem, "\n")] = '\0';
	}

	printf("%s\n", totalMem);

	fclose(f);
	return 0;
}