
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
	if (argc < 2) {
		puts("Usage: docwt <file>");
		return 120; // wrong command format
	}

	puts("Welcome to Document Writer.");
	puts("You can use :l(line) e.g. :l30 to jump to a certain line,");
	puts(":s to save your file, :c to copy/continue,");
	puts("or :q to quit.");

	FILE *f = fopen(argv[1], "r");
	if (!f) {
		return 27; // file fail
	}

	int lines = 0;

	char **filebuf = malloc(lines * sizeof(char *));
	if (!filebuf) {return 54;}

	char l[1024] = {0};

	// allocate each line
	while (fgets(l, 1023, f)) {
		filebuf[lines] = malloc(1024);
		if (!filebuf[lines]) {return 54;}
		strncpy(filebuf[lines], l, 1023);
		lines++;
	}

	printf("Previously %d lines in %s\n", lines, argv[1]);

	fclose(f);

	f = fopen(argv[1], "w");
	if (!f) {
		return 27; // file fail
	}

	int currentLine = 0;

	while (1) {
		if (currentLine == lines) {
			lines++;
			filebuf = realloc(filebuf, lines * sizeof(char *));
			filebuf[lines-1] = malloc(1024);
			filebuf[lines-1][0] = '\0';
		}

		fputs(filebuf[currentLine], stdout);
		fgets(l, 1023, stdin);

		if (l[0] == ':' && l[1] == 'l') {
			char *num = l + 2;
			currentLine = atoi(num);
			continue;
		} else if (l[0] == ':' && l[1] == 's') {
			for (int i = 0; i < lines; i++) {fputs(filebuf[i], f);}
			continue;
		} else if (l[0] == ':' && l[1] == 'q') {
			fclose(f);
			return 0;
		} else if (l[0] == ':' && l[1] == 'c') {
			currentLine++;
			continue;
		}

		strncpy(filebuf[currentLine], l, 1023);
		currentLine++;
	}
}