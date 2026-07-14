CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic

COMMON = socket_helpers.c http_parse.c urlencoded.c query_parse.c \
	 header_parse.c body_parse.c response.c todo_store.c
HEADERS = todo_api.h

all: 0-server 1-server 2-client 3-server \
	todo_api_0 todo_api_1 todo_api_2 todo_api_3 todo_api_4 todo_api_5

0-server: 0-server.c
	$(CC) $(CFLAGS) -o 0-server 0-server.c

1-server: 1-server.c
	$(CC) $(CFLAGS) -o 1-server 1-server.c

2-client: 2-client.c
	$(CC) $(CFLAGS) -o 2-client 2-client.c

3-server: 3-server.c
	$(CC) $(CFLAGS) -o 3-server 3-server.c

todo_api_0: todo_api_0.c $(COMMON) $(HEADERS)
	$(CC) $(CFLAGS) -o todo_api_0 todo_api_0.c $(COMMON)

todo_api_1: todo_api_1.c $(COMMON) $(HEADERS)
	$(CC) $(CFLAGS) -o todo_api_1 todo_api_1.c $(COMMON)

todo_api_2: todo_api_2.c $(COMMON) $(HEADERS)
	$(CC) $(CFLAGS) -o todo_api_2 todo_api_2.c $(COMMON)

todo_api_3: todo_api_3.c $(COMMON) $(HEADERS)
	$(CC) $(CFLAGS) -o todo_api_3 todo_api_3.c $(COMMON)

todo_api_4: todo_api_4.c $(COMMON) $(HEADERS)
	$(CC) $(CFLAGS) -o todo_api_4 todo_api_4.c $(COMMON)

todo_api_5: todo_api_5.c $(COMMON) $(HEADERS)
	$(CC) $(CFLAGS) -o todo_api_5 todo_api_5.c $(COMMON)

clean:
	rm -f 0-server 1-server 2-client 3-server \
		todo_api_0 todo_api_1 todo_api_2 todo_api_3 todo_api_4 todo_api_5

.PHONY: all clean
