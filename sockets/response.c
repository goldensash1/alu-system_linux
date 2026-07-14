#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include "todo_api.h"

/**
 * send_all - sends the full contents of a buffer over a socket, looping
 * over send(2) until everything has been sent
 * @fd: the socket's file descriptor
 * @buf: the buffer to send
 * @len: the number of bytes to send
 *
 * Return: the total number of bytes sent, or -1 on error
 */
ssize_t send_all(int fd, const char *buf, size_t len)
{
	size_t sent = 0;
	ssize_t n;

	while (sent < len)
	{
		n = send(fd, buf + sent, len - sent, 0);
		if (n <= 0)
			return (-1);
		sent += n;
	}
	return ((ssize_t)sent);
}

/**
 * send_response - builds and sends a full HTTP/1.1 response
 * @fd: the connected socket's file descriptor
 * @code: the HTTP status code
 * @reason: the HTTP status reason phrase
 * @content_type: the Content-Type header value, or NULL to omit it
 * @body: the response body, or NULL for an empty body
 * @body_len: the length of body
 */
void send_response(int fd, int code, const char *reason,
	const char *content_type, const char *body, size_t body_len)
{
	char header[1024];
	int header_len;
	char length_line[64];

	length_line[0] = '\0';
	if (content_type)
		snprintf(length_line, sizeof(length_line),
			"Content-Length: %zu\r\n", body_len);

	header_len = snprintf(header, sizeof(header),
		"HTTP/1.1 %d %s\r\n"
		"%s"
		"%s%s%s"
		"\r\n",
		code, reason, length_line,
		content_type ? "Content-Type: " : "",
		content_type ? content_type : "",
		content_type ? "\r\n" : "");

	send_all(fd, header, (size_t)header_len);
	if (body && body_len > 0)
		send_all(fd, body, body_len);
}
