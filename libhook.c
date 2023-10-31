
#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>
#include <string.h>


int puts(const char *s)
{
	int (*func) (const char *msg);

	func = dlsym(RTLD_NEXT, "puts");

	if (!strcmp(s, "123zegikniet"))
	{
		return func("overwritten");
	}
	return func(s);
}
