#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 12345

/**
 * open_listening_socket - opens an IPv4/TCP socket, binds it to the
 * given port on any address, and starts listening on it
 * @port: the port to listen on
 *
 * Return: the listening socket's file descriptor
 */
static int open_listening_socket(int port)
{
	int server_fd;
	struct sockaddr_in addr;

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);

	if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
	{
		perror("bind");
		close(server_fd);
		exit(EXIT_FAILURE);
	}

	if (listen(server_fd, 128) == -1)
	{
		perror("listen");
		close(server_fd);
		exit(EXIT_FAILURE);
	}

	return (server_fd);
}

/**
 * main - opens an IPv4/TCP socket and listens on port 12345 (any address)
 *
 * Return: EXIT_SUCCESS
 */
int main(void)
{
	int server_fd;

	setvbuf(stdout, NULL, _IOLBF, 0);

	server_fd = open_listening_socket(PORT);
	printf("Server listening on port %d\n", PORT);

	while (1)
		pause();

	close(server_fd);
	return (EXIT_SUCCESS);
}
