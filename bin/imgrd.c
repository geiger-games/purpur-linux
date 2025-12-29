
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// prints a string `n` times.
void putsrpt(const char *s, int times) {
	for (int i = 0; i < times; i++) {
		write(STDOUT_FILENO, s, strlen(s));
	}
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		puts("Usage: imgrd <file>");
		return 120; // wrong command format
	}

	FILE *f = fopen(argv[1], "rb");
	if (!f) {
		return 27; // file fail
	}

	char buf[256] = {0};
	int type;
	int amount;

	while (fgets(buf, 255, f)) {
		for (int i = 0; i < strlen(buf); i++) {
			type = buf[i] >> 7;
			amount = buf[i] & 0b0111111;
			putsrpt(type ? "██" : "  ", amount);
		}
		puts(" ");
	}

	return 0;
}