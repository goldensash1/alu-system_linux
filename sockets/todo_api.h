#ifndef TODO_API_H
#define TODO_API_H

#include <stddef.h>
#include <sys/types.h>

#define BUF_SIZE 65536
#define MAX_PAIRS 32
#define MAX_TODOS 1024

/**
 * struct pair_s - a generic key/value pair
 * @key: the key
 * @value: the value
 */
typedef struct pair_s
{
	char key[256];
	char value[1024];
} pair_t;

/**
 * struct http_request_s - a parsed HTTP request
 * @method: the HTTP method (GET, POST, ...)
 * @path: the request path, without the query string
 * @query: the raw query string, without the leading '?'
 * @version: the HTTP version (HTTP/1.1)
 * @headers: the parsed headers
 * @header_count: number of headers parsed
 * @queries: the parsed query key/value pairs
 * @query_count: number of query pairs parsed
 * @body: the raw request body
 * @body_params: the parsed body key/value pairs
 * @body_count: number of body pairs parsed
 */
typedef struct http_request_s
{
	char method[16];
	char path[256];
	char query[1024];
	char version[16];
	pair_t headers[MAX_PAIRS];
	int header_count;
	pair_t queries[MAX_PAIRS];
	int query_count;
	char body[BUF_SIZE];
	pair_t body_params[MAX_PAIRS];
	int body_count;
} http_request_t;

/**
 * struct todo_s - a single TODO item
 * @id: the todo's unique id
 * @title: the todo's title
 * @description: the todo's description
 */
typedef struct todo_s
{
	int id;
	char title[1024];
	char description[1024];
} todo_t;

/* socket_helpers.c */
int create_server_socket(int port);
int accept_client(int server_fd, char *ip_buf, size_t ip_buf_len);

/* http_parse.c */
int read_request(int fd, char *buffer, size_t bufsize);
long get_content_length(const char *buffer);
void parse_request_line(const char *buffer, http_request_t *req);

/* urlencoded.c */
void parse_pairs(const char *str, pair_t *pairs, int *count, int max);

/* query_parse.c */
void parse_query(http_request_t *req);
void print_queries(const http_request_t *req);

/* header_parse.c */
void parse_headers(const char *buffer, http_request_t *req);
void print_headers(const http_request_t *req);
const char *get_header(const http_request_t *req, const char *name);

/* body_parse.c */
void parse_body(const char *buffer, http_request_t *req);
void print_body_params(const http_request_t *req);

/* response.c */
ssize_t send_all(int fd, const char *buf, size_t len);
void send_response(int fd, int code, const char *reason,
	const char *content_type, const char *body, size_t body_len);

/* todo_store.c */
int create_todo(const char *title, const char *description);
int todo_to_json(const todo_t *t, char *buf, size_t bufsize);
int todos_to_json(char *buf, size_t bufsize);

#endif /* TODO_API_H */
