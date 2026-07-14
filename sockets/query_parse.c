#include <stdio.h>
#include "todo_api.h"

/**
 * parse_query - parses the raw query string of a request into key/value
 * pairs
 * @req: the http_request_t whose query field has already been set
 */
void parse_query(http_request_t *req)
{
	parse_pairs(req->query, req->queries, &req->query_count, MAX_PAIRS);
}

/**
 * print_queries - prints the parsed query key/value pairs of a request
 * @req: the http_request_t to print the queries of
 */
void print_queries(const http_request_t *req)
{
	int i;

	for (i = 0; i < req->query_count; i++)
		printf("Query: \"%s\" -> \"%s\"\n",
			req->queries[i].key, req->queries[i].value);
}
