#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 12345
#define BUF_SIZE 4096

/**
 * main - opens an IPv4/TCP socket, listens on port 12345 (any address),
 * accepts a single connection, prints the client's IP address and the
 * message it sends, then exits
 *
 * Return: EXIT_SUCCESS, or EXIT_FAILURE on error
 */
int main(void)
{
	int server_fd, client_fd;
	struct sockaddr_in addr, client_addr;
	socklen_t addr_len = sizeof(client_addr);
	char ip[INET_ADDRSTRLEN];
	char buffer[BUF_SIZE];
	ssize_t n;

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

	client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
	if (client_fd == -1)
	{
		perror("accept");
		close(server_fd);
		return (EXIT_FAILURE);
	}

	inet_ntop(AF_INET, &client_addr.sin_addr, ip, sizeof(ip));
	printf("Client connected: %s\n", ip);

	n = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
	if (n > 0)
	{
		buffer[n] = '\0';
		printf("Message received: \"%s\"\n", buffer);
	}

	close(client_fd);
	close(server_fd);
	return (EXIT_SUCCESS);
}
