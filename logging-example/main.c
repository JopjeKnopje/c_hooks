#include <unistd.h>

int print(const char *s)
{
	size_t i = 0;
	while (s[i])
		i++;
	return write(STDOUT_FILENO, s, i);
}

int main()
{
	print("Hello World\n");
	return 0;
}
