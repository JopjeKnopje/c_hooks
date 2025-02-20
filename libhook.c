#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>
#include <string.h>
#include <fcntl.h>

// lovely syntax right here.
static int (*get_write(void))(int, const void *, size_t)
{
	static int (*real_write) (int fildes, const void *buf, size_t nbyte) = NULL;
	if (!real_write)
		real_write = dlsym(RTLD_NEXT, "write");
	
	return real_write;
}


static void write_to_file(const char *f, const char *s)
{
	int fd = open(f, O_RDWR | O_APPEND | O_CREAT, 0666);

	get_write()(fd, s, strlen(s));
	close(fd);
}


ssize_t write(int fildes, const void *buf, size_t nbyte)
{
	int (*actual_write) (int fildes, const void *buf, size_t nbyte) = get_write();
	// actual_write = dlsym(RTLD_NEXT, "write");


	// char *s =  "overwritten\n\0";
	const char *s = buf;
	write_to_file("hihi_log.txt", s);
	// actual_write(STDOUT_FILENO, s, strlen(s));
	return actual_write(fildes, buf, nbyte);
}
