
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include <unistd.h>
	#include <sys/types.h>
	#include <sys/wait.h>

#define TERM_CLEAR() write(STDOUT_FILENO, "\033[2J\033[H", 7)

int main(void) {
	char buf[256] = {0};
	char binPath[128] = {0};
	int retStatus = 0;
	while (1) {
		write(STDOUT_FILENO, "push> ", 6);
		if (fgets(buf, 255, stdin)) {
			buf[strcspn(buf, "\n")] = '\0';
		}

		char *cmd = strtok(buf, " ");
		char *arg1 = strtok(NULL, " ");
		char *arg2 = strtok(NULL, " ");
		char *arg3 = strtok(NULL, " ");
		char *arg4 = strtok(NULL, " ");

		if (!strcmp(cmd, "q") || !strcmp(cmd, "quit") || !strcmp(cmd, "exit")) {
			break;
		}
		if (!strcmp(cmd, "cls") || !strcmp(cmd, "clear")) {
			TERM_CLEAR();
		}

		snprintf(binPath, 127, "/bin/%s", cmd);

		pid_t pid = fork();
		if (pid == 0) {
			execl(binPath, cmd, arg1, arg2, arg3, arg4, NULL);
			perror("exec");
			_exit(127);
		}

		waitpid(pid, &retStatus, 0);
		retStatus = WEXITSTATUS(retStatus);
		switch (retStatus) {
			case 0:   puts("[ INFO ] Success"); break;
			case 54:  puts("[ INFO ] Allocation failed"); break;
			case 27:  puts("[ INFO ] File/directory failed to open"); break;
			case 120: puts("[ INFO ] Wrong command format"); break;
			case 255: break; // silent error
			case 1:   puts("[ INFO ] Other fail"); break;
		}
	}
	return 0;
}