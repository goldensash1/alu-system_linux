#include <stdio.h>
#include <string.h>
#include "todo_api.h"

static todo_t todos[MAX_TODOS];
static int todo_count;

/**
 * create_todo - creates a new todo and adds it to the in-memory list
 * @title: the todo's title
 * @description: the todo's description
 *
 * Return: the new todo's id
 */
int create_todo(const char *title, const char *description)
{
	todo_t *t = &todos[todo_count];

	t->id = todo_count;
	strncpy(t->title, title, sizeof(t->title) - 1);
	t->title[sizeof(t->title) - 1] = '\0';
	strncpy(t->description, description, sizeof(t->description) - 1);
	t->description[sizeof(t->description) - 1] = '\0';

	todo_count++;
	return (t->id);
}

/**
 * todo_to_json - writes the JSON representation of a single todo
 * @t: the todo to serialize
 * @buf: the buffer to write into
 * @bufsize: the size of buf
 *
 * Return: the number of characters written
 */
int todo_to_json(const todo_t *t, char *buf, size_t bufsize)
{
	return (snprintf(buf, bufsize,
		"{\"id\":%d,\"title\":\"%s\",\"description\":\"%s\"}",
		t->id, t->title, t->description));
}

/**
 * todos_to_json - writes the JSON representation of the list of all todos
 * @buf: the buffer to write into
 * @bufsize: the size of buf
 *
 * Return: the number of characters written
 */
int todos_to_json(char *buf, size_t bufsize)
{
	size_t offset = 0;
	int i;

	offset += snprintf(buf + offset, bufsize - offset, "[");
	for (i = 0; i < todo_count; i++)
	{
		if (i > 0)
			offset += snprintf(buf + offset, bufsize - offset, ",");
		offset += todo_to_json(&todos[i], buf + offset, bufsize - offset);
	}
	offset += snprintf(buf + offset, bufsize - offset, "]");

	return ((int)offset);
}
