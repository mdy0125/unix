#include "apue.h"
#include <fcntl.h>

static int lockabyte(int fd, const char *name, off_t offset)
{
	if (writew_lock(fd, offset, SEEK_SET, 1) < 0)
		printf("writew_lock failed\n");
	printf("%s: got the lock, byte %lld\n", name, (long long)offset);
}

int main(int argc, char const *argv[])
{
	int fd;
	pid_t pid;

	if ((fd = create("templock", FILE_MODE)) < 0)
		printf("create error\n");
	if (write(fd, "ab", 2) < 0)
	{
		/* code */
		printf("write failed\n");
	}

	TELL_WAIT();
	if ((pid = fork()) < 0)
		printf("fork failed\n");
	else if (pid == 0)
	{
		lockabyte(fd, "child", 0);
		TELL_PARENT(getpid());
		WAIT_PARENT();
		lockabyte(fd, "child", 1);
	}
	else
	{
		lockabyte(fd, "parent", 1);
		TELL_CHILD(pid);
		WAIT_CHILD();
		lockabyte(fd, "parent", 0);
	}

	exit(0);
	return 0;
}