#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "todo_api.h"

#define PORT 8080

/**
 * main - opens an IPv4/TCP socket on port 8080, and for every incoming
 * connection prints the client's IP, the raw HTTP request and the
 * breakdown of its request line, then responds with a 200 OK
 *
 * Return: EXIT_SUCCESS
 */
int main(void)
{
	int server_fd, client_fd;
	char ip[64];
	char buffer[BUF_SIZE];
	http_request_t req;

	setvbuf(stdout, NULL, _IOLBF, 0);
	server_fd = create_server_socket(PORT);
	printf("Server listening on port %d\n", PORT);

	while (1)
	{
		client_fd = accept_client(server_fd, ip, sizeof(ip));
		if (client_fd == -1)
			continue;
		printf("Client connected: %s\n", ip);

		read_request(client_fd, buffer, sizeof(buffer));
		printf("Raw request: \"%s\"\n", buffer);

		parse_request_line(buffer, &req);
		printf("Method: %s\n", req.method);
		printf("Path: %s\n", req.path);
		printf("Version: %s\n", req.version);

		send_response(client_fd, 200, "OK", NULL, NULL, 0);
		close(client_fd);
	}

	close(server_fd);
	return (EXIT_SUCCESS);
}
