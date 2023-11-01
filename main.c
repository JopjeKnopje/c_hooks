#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


int main()
{
	char *s = "string 123\n\0";
	write(STDOUT_FILENO, s, strlen(s));
	return 0;
}
