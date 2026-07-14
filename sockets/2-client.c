#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>

/**
 * connect_to_host - resolves a host/port and connects a TCP socket to it
 * @host: the host to connect to
 * @port: the port to connect to
 *
 * Return: the connected socket's file descriptor
 */
static int connect_to_host(const char *host, const char *port)
{
	int sock_fd;
	struct addrinfo hints, *res;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	if (getaddrinfo(host, port, &hints, &res) != 0)
	{
		fprintf(stderr, "Could not resolve host: %s\n", host);
		exit(EXIT_FAILURE);
	}

	sock_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (sock_fd == -1)
	{
		perror("socket");
		freeaddrinfo(res);
		exit(EXIT_FAILURE);
	}

	if (connect(sock_fd, res->ai_addr, res->ai_addrlen) == -1)
	{
		perror("connect");
		freeaddrinfo(res);
		close(sock_fd);
		exit(EXIT_FAILURE);
	}

	freeaddrinfo(res);
	return (sock_fd);
}

/**
 * main - connects to a listening TCP server
 * @argc: argument count
 * @argv: argument vector, argv[1] is the host, argv[2] is the port
 *
 * Return: EXIT_SUCCESS, or EXIT_FAILURE on error
 */
int main(int argc, char *argv[])
{
	int sock_fd;

	if (argc < 3)
	{
		fprintf(stderr, "Usage: %s <host> <port>\n", argv[0]);
		return (EXIT_FAILURE);
	}

	sock_fd = connect_to_host(argv[1], argv[2]);
	printf("Connected to %s:%s\n", argv[1], argv[2]);

	close(sock_fd);
	return (EXIT_SUCCESS);
}
