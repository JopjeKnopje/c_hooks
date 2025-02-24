#define _GNU_SOURCE

#include <stdio.h>
#include <dlfcn.h>


ssize_t write(int fildes, const void *buf, size_t nbyte)
{
	// write's function signature.
	int (*actual_write) (int fildes, const void *buf, size_t nbyte);

	actual_write = dlsym(RTLD_NEXT, "write");

	char *s =  "overwritten\n\0";
	return actual_write(fildes, s, nbyte);
}
