#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 12345

/**
 * main - opens an IPv4/TCP socket and listens on port 12345 (any address)
 *
 * Return: EXIT_SUCCESS, or EXIT_FAILURE on error
 */
int main(void)
{
	int server_fd;
	struct sockaddr_in addr;

	setvbuf(stdout, NULL, _IOLBF, 0);

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1)
	{
		perror("socket");
		return (EXIT_FAILURE);
	}

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(PORT);

	if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
	{
		perror("bind");
		close(server_fd);
		return (EXIT_FAILURE);
	}

	if (listen(server_fd, 128) == -1)
	{
		perror("listen");
		close(server_fd);
		return (EXIT_FAILURE);
	}

	printf("Server listening on port %d\n", PORT);

	while (1)
		pause();

	close(server_fd);
	return (EXIT_SUCCESS);
}
