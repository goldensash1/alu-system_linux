# Sockets

A minimal introduction to BSD sockets, culminating in a small HTTP REST API
for managing an in-memory TODO list.

## Environment

* OS: **Ubuntu 14.04 LTS**, compiled with **gcc 4.8.4**
* Flags: `-Wall -Werror -Wextra -pedantic`
* Allowed editors: `vi`, `vim`, `emacs`
* All files end with a new line

## Files

| File | Description |
| --- | --- |
| `0-server.c` | Opens a socket and listens on port `12345` |
| `1-server.c` | Accepts a connection and prints the client's IP address |
| `2-client.c` | Connects to a listening server (`<host> <port>`) |
| `3-server.c` | Accepts a connection, prints the client's IP and the message it sends |
| `todo_api.h` | Shared types/prototypes for the REST API programs |
| `socket_helpers.c` | Creates a listening socket, accepts client connections |
| `http_parse.c` | Reads a full HTTP request off the wire, parses its request line |
| `urlencoded.c` | Splits a URL-encoded `key=value&...` string into pairs |
| `query_parse.c` | Parses/prints a request's query string |
| `header_parse.c` | Parses/prints/looks up a request's headers |
| `body_parse.c` | Parses/prints a request's body parameters |
| `response.c` | Builds and sends an HTTP/1.1 response |
| `todo_store.c` | In-memory TODO storage and JSON serialization |
| `todo_api_0.c` | Prints the raw request and its request-line breakdown |
| `todo_api_1.c` | Prints the request path and its query parameters |
| `todo_api_2.c` | Prints the request's headers |
| `todo_api_3.c` | Prints the request path and its body parameters |
| `todo_api_4.c` | Implements `POST /todos`; `404` for anything else |
| `todo_api_5.c` | Implements `POST /todos` and `GET /todos`; `404` for anything else |
| `Makefile` | Builds all the programs above |

## REST API

The TODO REST API keeps its data in memory only (nothing is persisted to
disk); the list is empty every time the server starts.

A `todo` has the fields `id` (positive integer, starting at `0`), `title`
and `description` (both strings).

| Route | Method | Description |
| --- | --- | --- |
| `/todos` | `POST` | Creates a todo from the `title` and `description` body parameters |
| `/todos` | `GET` | Retrieves the list of all todos |

## Compilation

```
make              # builds every program
make todo_api_5   # builds a single program
make clean        # removes all built binaries
```

## Usage

```
./0-server
./1-server
./2-client <host> <port>
./3-server
./todo_api_0   # through todo_api_5, all listen on port 8080
```
