#define _GNU_SOURCE
#include <stdio.h>
#include <dirent.h>

int main(int argc, char *argv[]) {
	if (argc == 1) {
		puts("Usage: listd <directory>");
		return 120; // wrong command format
	}

	struct dirent *de;

	DIR *dir = opendir(argv[1]);

	if (!dir)
		return 27; // file fail

	int a = 0;

	while ((de = readdir(dir)) != NULL) {
		if (a == 5) {
			a = 0;
			puts("|");
		}
		printf("| %-20s:%s ", de->d_name, de->d_type == DT_DIR ? "dir " : (de->d_type == DT_REG ? "file" : "unkn"));
		a++;
	}
	puts("|");

	closedir(dir);    
	return 0;
}