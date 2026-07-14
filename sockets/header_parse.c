#include <stdio.h>
#include <string.h>
#include <strings.h>
#include "todo_api.h"

/**
 * parse_headers - parses the header lines of a raw HTTP request
 * @buffer: the raw HTTP request
 * @req: the http_request_t to fill in
 */
void parse_headers(const char *buffer, http_request_t *req)
{
	const char *line = strstr(buffer, "\n");
	const char *colon;
	const char *value;
	const char *eol;
	size_t key_len;

	req->header_count = 0;
	if (line)
		line++;

	while (line && *line && line[0] != '\r' && line[0] != '\n' &&
		req->header_count < MAX_PAIRS)
	{
		eol = strstr(line, "\n");
		colon = strchr(line, ':');
		if (colon && (!eol || colon < eol))
		{
			key_len = colon - line;
			if (key_len >= sizeof(req->headers[0].key))
				key_len = sizeof(req->headers[0].key) - 1;
			memcpy(req->headers[req->header_count].key, line, key_len);
			req->headers[req->header_count].key[key_len] = '\0';

			value = colon + 1;
			while (*value == ' ')
				value++;
			key_len = (eol ? (size_t)(eol - value) : strlen(value));
			if (key_len > 0 && value[key_len - 1] == '\r')
				key_len--;
			if (key_len >= sizeof(req->headers[0].value))
				key_len = sizeof(req->headers[0].value) - 1;
			memcpy(req->headers[req->header_count].value, value, key_len);
			req->headers[req->header_count].value[key_len] = '\0';

			req->header_count++;
		}
		line = eol ? eol + 1 : NULL;
	}
}

/**
 * print_headers - prints the parsed header key/value pairs of a request
 * @req: the http_request_t to print the headers of
 */
void print_headers(const http_request_t *req)
{
	int i;

	for (i = 0; i < req->header_count; i++)
		printf("Header: \"%s\" -> \"%s\"\n",
			req->headers[i].key, req->headers[i].value);
}

/**
 * get_header - looks up a header by name, case-insensitively
 * @req: the http_request_t to search
 * @name: the header name to look for
 *
 * Return: the header's value, or NULL if not found
 */
const char *get_header(const http_request_t *req, const char *name)
{
	int i;

	for (i = 0; i < req->header_count; i++)
		if (strcasecmp(req->headers[i].key, name) == 0)
			return (req->headers[i].value);
	return (NULL);
}
