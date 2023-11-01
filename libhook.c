#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>
#include <string.h>



ssize_t write(int fildes, const void *buf, size_t nbyte)
{
	int (*func) (int fildes, const void *buf, size_t nbyte);
	func = dlsym(RTLD_NEXT, "write");

	char *s =  "overwritten\n\0";
	func(STDOUT_FILENO, s, strlen(s));
	return func(fildes, buf, nbyte);
}
