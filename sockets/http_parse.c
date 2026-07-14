#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/socket.h>
#include "todo_api.h"

/**
 * get_content_length - looks up the Content-Length header in a raw,
 * not yet parsed, HTTP request
 * @buffer: the raw HTTP request
 *
 * Return: the content length, or 0 if the header is absent
 */
long get_content_length(const char *buffer)
{
	const char *line = buffer;
	const char *colon;

	while (line && *line && line[0] != '\r' && line[0] != '\n')
	{
		if (strncasecmp(line, "Content-Length:", 15) == 0)
		{
			colon = line + 15;
			while (*colon == ' ')
				colon++;
			return (atol(colon));
		}
		line = strstr(line, "\n");
		if (line)
			line++;
	}
	return (0);
}

/**
 * read_request - reads a full HTTP request (headers and, if any, body)
 * from a connected socket into buffer
 * @fd: the connected socket's file descriptor
 * @buffer: the buffer to store the request in
 * @bufsize: the size of buffer
 *
 * Return: the total number of bytes read, or -1 on error
 */
int read_request(int fd, char *buffer, size_t bufsize)
{
	size_t total = 0;
	ssize_t n;
	char *headers_end = NULL;
	long content_length = 0;
	size_t body_have;

	while (total < bufsize - 1)
	{
		n = recv(fd, buffer + total, bufsize - 1 - total, 0);
		if (n <= 0)
			break;
		total += n;
		buffer[total] = '\0';

		if (!headers_end)
		{
			headers_end = strstr(buffer, "\r\n\r\n");
			if (headers_end)
			{
				headers_end += 4;
				content_length = get_content_length(buffer);
			}
		}

		if (headers_end)
		{
			body_have = total - (size_t)(headers_end - buffer);
			if ((long)body_have >= content_length)
				break;
		}
	}

	return ((int)total);
}

/**
 * parse_request_line - parses the request line (method, path, query
 * string and HTTP version) of a raw HTTP request
 * @buffer: the raw HTTP request
 * @req: the http_request_t to fill in
 */
void parse_request_line(const char *buffer, http_request_t *req)
{
	char line[1024];
	char raw_path[512];
	char *question_mark;
	size_t i;

	for (i = 0; i < sizeof(line) - 1 && buffer[i] &&
		buffer[i] != '\r' && buffer[i] != '\n'; i++)
		line[i] = buffer[i];
	line[i] = '\0';

	req->method[0] = '\0';
	req->path[0] = '\0';
	req->query[0] = '\0';
	req->version[0] = '\0';

	sscanf(line, "%15s %511s %15s", req->method, raw_path, req->version);

	question_mark = strchr(raw_path, '?');
	if (question_mark)
	{
		*question_mark = '\0';
		strncpy(req->query, question_mark + 1, sizeof(req->query) - 1);
	}
	strncpy(req->path, raw_path, sizeof(req->path) - 1);
}
