
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
	if (argc < 4) {
		puts("Usage: calc <n1> <op> <n2>");
		return 120; // wrong command format
	}

	double n1 = atof(argv[1]);
	double n2 = atof(argv[3]);
	double result = 0;
	char *op = argv[2];

	if (!strcmp(op, "+")) {
		result = n1 + n2;
	} else if (!strcmp(op, "-")) {
		result = n1 - n2;
	} else if (!strcmp(op, "*")) {
		result = n1 * n2;
	} else if (!strcmp(op, "/")) {
		result = n1 / n2;
	}

	printf("%.2f %s %.2f = %.2f", n1, op, n2, result);

	return 0;
}