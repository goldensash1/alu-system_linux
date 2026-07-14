#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "todo_api.h"

#define PORT 8080

/**
 * get_body_value - looks up a body parameter by key
 * @req: the http_request_t to search
 * @key: the key to look for
 *
 * Return: the parameter's value, or NULL if not found
 */
static const char *get_body_value(const http_request_t *req, const char *key)
{
	int i;

	for (i = 0; i < req->body_count; i++)
		if (strcmp(req->body_params[i].key, key) == 0)
			return (req->body_params[i].value);
	return (NULL);
}

/**
 * handle_create_todo - handles POST /todos: creates a new todo from the
 * request's body parameters and responds accordingly
 * @client_fd: the connected client's file descriptor
 * @req: the parsed HTTP request
 * @ip: the client's IP address, for logging
 */
static void handle_create_todo(int client_fd, const http_request_t *req,
	const char *ip)
{
	const char *title, *description;
	char body[BUF_SIZE];
	int len;
	todo_t t;

	if (!get_header(req, "Content-Length"))
	{
		printf("%s POST /todos -> 411 Length Required\n", ip);
		send_response(client_fd, 411, "Length Required", NULL, NULL, 0);
		return;
	}

	title = get_body_value(req, "title");
	description = get_body_value(req, "description");
	if (!title || !title[0] || !description || !description[0])
	{
		printf("%s POST /todos -> 422 Unprocessable Entity\n", ip);
		send_response(client_fd, 422, "Unprocessable Entity", NULL, NULL, 0);
		return;
	}

	t.id = create_todo(title, description);
	strncpy(t.title, title, sizeof(t.title) - 1);
	strncpy(t.description, description, sizeof(t.description) - 1);
	len = todo_to_json(&t, body, sizeof(body));

	printf("%s POST /todos -> 201 Created\n", ip);
	send_response(client_fd, 201, "Created", "application/json",
		body, (size_t)len);
}

/**
 * handle_list_todos - handles GET /todos: responds with the JSON
 * representation of every todo currently stored
 * @client_fd: the connected client's file descriptor
 * @ip: the client's IP address, for logging
 */
static void handle_list_todos(int client_fd, const char *ip)
{
	char body[BUF_SIZE];
	int len = todos_to_json(body, sizeof(body));

	printf("%s GET /todos -> 200 OK\n", ip);
	send_response(client_fd, 200, "OK", "application/json", body, (size_t)len);
}

/**
 * main - opens an IPv4/TCP socket on port 8080 and implements the
 * POST /todos and GET /todos routes of the TODO REST API, responding
 * 404 to any other route or method
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

		read_request(client_fd, buffer, sizeof(buffer));
		parse_request_line(buffer, &req);
		parse_headers(buffer, &req);
		parse_body(buffer, &req);

		if (strcmp(req.method, "POST") == 0 && strcmp(req.path, "/todos") == 0)
			handle_create_todo(client_fd, &req, ip);
		else if (strcmp(req.method, "GET") == 0 &&
			strcmp(req.path, "/todos") == 0)
			handle_list_todos(client_fd, ip);
		else
		{
			printf("%s %s %s -> 404 Not Found\n", ip, req.method, req.path);
			send_response(client_fd, 404, "Not Found", NULL, NULL, 0);
		}

		close(client_fd);
	}

	close(server_fd);
	return (EXIT_SUCCESS);
}
