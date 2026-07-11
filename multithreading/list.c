#include <stdlib.h>
#include "list.h"

/**
 * list_init - Initialize a doubly linked list
 *
 * @list: Pointer to the list to initialize
 *
 * Return: Pointer to the initialized list, or NULL on failure
 */
list_t *list_init(list_t *list)
{
	if (!list)
		return (NULL);
	list->size = 0;
	list->head = NULL;
	list->tail = NULL;
	return (list);
}

/**
 * list_add - Add a new node holding @content at the end of @list
 *
 * @list:    Pointer to the list to add a node to
 * @content: Content to store in the new node
 *
 * Return: Pointer to the newly created node, or NULL on failure
 */
node_t *list_add(list_t *list, void *content)
{
	node_t *node;

	if (!list)
		return (NULL);
	node = malloc(sizeof(*node));
	if (!node)
		return (NULL);
	node->content = content;
	node->next = NULL;
	node->prev = list->tail;
	if (list->tail)
		list->tail->next = node;
	else
		list->head = node;
	list->tail = node;
	list->size++;
	return (node);
}

/**
 * list_destroy - Empty a list, freeing all its nodes
 *
 * @list:      Pointer to the list to empty
 * @free_func: Function used to free each node's content (can be NULL)
 */
void list_destroy(list_t *list, node_func_t free_func)
{
	node_t *node, *next;

	if (!list)
		return;
	for (node = list->head; node; node = next)
	{
		next = node->next;
		if (free_func)
			free_func(node->content);
		free(node);
	}
	list->size = 0;
	list->head = NULL;
	list->tail = NULL;
}

/**
 * list_each - Apply a function to the content of each node of a list
 *
 * @list:   Pointer to the list to iterate over
 * @action: Function to apply to each node's content
 */
void list_each(list_t const *list, node_func_t action)
{
	node_t *node;

	if (!list || !action)
		return;
	for (node = list->head; node; node = node->next)
		action(node->content);
}
