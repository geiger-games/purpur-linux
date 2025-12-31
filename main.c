
// yes, this is literally supposed to be a kernel ;-;

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/reboot.h>
#include <linux/reboot.h>
#include <signal.h>
#include <termios.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/types.h>
#include <stdint.h>
#include <sys/mount.h>

#define TERM_CLEAR() write(STDOUT_FILENO, "\033[2J\033[H", 7)

void sigchld_handler(int sig) {
	(void)sig;
	while (waitpid(-1, NULL, WNOHANG) > 0) {}
}

void term_setup(void) {
	struct termios term;
	if (tcgetattr(STDIN_FILENO, &term) < 0) {
		perror("tcgetattr");
		return;
	}

	term.c_lflag |= (ICANON | ECHO | ISIG);
	term.c_iflag |= ICRNL;
	term.c_oflag |= OPOST;

	if (tcsetattr(STDIN_FILENO, TCSANOW, &term) < 0) {
		perror("tcsetattr");
		return;
	}
}

int main() {
	// mount
	if (mount("proc", "/proc", "proc", 0, NULL) < 0) {
		perror("mount /proc failed");
	}

	if (mount("devtmpfs", "/dev", "devtmpfs", 0, NULL) < 0) {
		perror("mount /dev failed");
	}

	// optional: mount sysfs if needed
	if (mount("sysfs", "/sys", "sysfs", 0, NULL) < 0) {
		perror("mount /sys failed");
	}

	signal(SIGCHLD, sigchld_handler);
	term_setup();

	TERM_CLEAR();

	puts("  /\\--=--/\\ \n"
		  " /  \\   /  \\ \n"
		  "|    |=|    |\n"
		  " \\  /   \\  /\n"
		  "  \\/--=--\\/");
   puts("Purpur Linux\n");

	pid_t pid = fork();

	if (pid == 0) {
		execl("/bin/push", "push", NULL);
		perror("exec shell failed");
		_exit(127);
	}

	int toshStatus;
	waitpid(pid, &toshStatus, 0);

	while (1) {reboot(LINUX_REBOOT_CMD_RESTART);}
}