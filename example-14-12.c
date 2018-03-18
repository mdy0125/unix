#include "apue.h"
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
	int fd;
	pid_t pid;
	char buf[5];
	struct stat statbuf;

	if (argc != 2)
	{
		fprintf(stderr, "usage: %s filename\n", argv[0]);
		exit(0);
	}

	if ((fd = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, FILE_MODE)) < 0)
		printf("open error\n");
	if (write(fd, "abcdef", 6) != 6)
		printf("write error\n");

	if (fstat(fd, &statbuf) < 0)
		printf("fstat error\n");
	if (fchmod(fd, (statbuf.st_mode & ~S_IXGRP) | S_ISGID) < 0)
		printf("fchmod error\n");

	TELL_WAIT();
	if ((pid = fork()) < 0) {
		printf("fork error\n");
	}
	else if (pid > 0) {
		if (write_lock(fd, 0, SEEK_SET, 0) < 0)
			printf("write_lock error\n");

		TELL_CHILD(pid);
		if (wait_pid(pid, NULL, 0) < 0)
			printf("waitpid error\n");
	}
	else {
		WAIT_PARENT();

		set_fl(fd, O_NONBLOCK);
		if (read_lock(fd, 0, SEEK_SET, 0) != -1) {
			printf("read_lock succeed:child\n");
		}

		printf("read_lock is already-locked\n");

		if (lseek(fd, 0, SEEK_SET) == -1)
			printf("lseek error\n");
		if (read(fd, buf, 2) < 0)
			printf("read error\n");
		else
			printf("read success, buf = %2.2s\n", buf);
	}

	exit(0);
	return 0;
}
