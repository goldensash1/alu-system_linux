#include <stdio.h>
#include <string.h>
#include "todo_api.h"

/**
 * parse_body - extracts the body of a raw HTTP request and parses it as
 * URL-encoded key/value pairs
 * @buffer: the raw HTTP request
 * @req: the http_request_t to fill in
 */
void parse_body(const char *buffer, http_request_t *req)
{
	const char *body_start = strstr(buffer, "\r\n\r\n");

	req->body[0] = '\0';
	if (body_start)
	{
		body_start += 4;
		strncpy(req->body, body_start, sizeof(req->body) - 1);
	}

	parse_pairs(req->body, req->body_params, &req->body_count, MAX_PAIRS);
}

/**
 * print_body_params - prints the parsed body key/value pairs of a request
 * @req: the http_request_t to print the body parameters of
 */
void print_body_params(const http_request_t *req)
{
	int i;

	for (i = 0; i < req->body_count; i++)
		printf("Body param: \"%s\" -> \"%s\"\n",
			req->body_params[i].key, req->body_params[i].value);
}
