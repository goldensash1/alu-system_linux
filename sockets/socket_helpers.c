#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "todo_api.h"

/**
 * create_server_socket - opens an IPv4/TCP socket, binds it to the given
 * port on any address, and starts listening on it
 * @port: the port to listen on
 *
 * Return: the listening socket's file descriptor
 */
int create_server_socket(int port)
{
	int server_fd;
	int optval = 1;
	struct sockaddr_in addr;

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}

	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

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
 * accept_client - accepts an incoming connection on a listening socket
 * @server_fd: the listening socket's file descriptor
 * @ip_buf: buffer to store the connected client's IP address
 * @ip_buf_len: size of ip_buf
 *
 * Return: the accepted client's file descriptor
 */
int accept_client(int server_fd, char *ip_buf, size_t ip_buf_len)
{
	int client_fd;
	struct sockaddr_in client_addr;
	socklen_t addr_len = sizeof(client_addr);

	client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
	if (client_fd == -1)
	{
		perror("accept");
		return (-1);
	}

	inet_ntop(AF_INET, &client_addr.sin_addr, ip_buf, ip_buf_len);

	return (client_fd);
}
