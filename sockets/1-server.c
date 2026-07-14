#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
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
 * main - opens an IPv4/TCP socket, listens on port 12345 (any address),
 * and prints the IP address of every client that connects
 *
 * Return: EXIT_SUCCESS
 */
int main(void)
{
	int server_fd, client_fd;
	struct sockaddr_in client_addr;
	socklen_t addr_len = sizeof(client_addr);
	char ip[INET_ADDRSTRLEN];

	setvbuf(stdout, NULL, _IOLBF, 0);

	server_fd = open_listening_socket(PORT);
	printf("Server listening on port %d\n", PORT);

	while (1)
	{
		client_fd = accept(server_fd, (struct sockaddr *)&client_addr,
			&addr_len);
		if (client_fd == -1)
		{
			perror("accept");
			continue;
		}

		inet_ntop(AF_INET, &client_addr.sin_addr, ip, sizeof(ip));
		printf("Client connected: %s\n", ip);

		close(client_fd);
	}

	close(server_fd);
	return (EXIT_SUCCESS);
}
