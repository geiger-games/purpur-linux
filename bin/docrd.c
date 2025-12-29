
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
	if (argc < 2) {
		puts("Usage: docrd <file>");
		return 120; // wrong command format
	}

	puts("Welcome to Document Reader.");
	puts("For every line, press [ENTER] for a new line,");
	puts("or q to exit immediately.");

	char line[1024] = {0};
	char i = 0;

	FILE *f = fopen(argv[1], "r");
	if (!f) {
		return 27; // file fail
	}

	while (fgets(line, sizeof(line), f)) {
		fputs(line, stdout);
		scanf("%c", &i);
		if (i == 'q') {
			break;
		}
	}

	fclose(f);

	return 0;
}