#ifndef LIST_H
#define LIST_H

#include <stddef.h>

/**
 * struct node_s - Node of a doubly linked list
 *
 * @content: Content of the node
 * @next:    Pointer to the next node in the list
 * @prev:    Pointer to the previous node in the list
 */
typedef struct node_s
{
	void		*content;
	struct node_s	*next;
	struct node_s	*prev;
} node_t;

/**
 * struct list_s - Doubly linked list
 *
 * @size: Number of nodes in the list
 * @head: Pointer to the head node of the list
 * @tail: Pointer to the tail node of the list
 */
typedef struct list_s
{
	size_t	size;
	node_t	*head;
	node_t	*tail;
} list_t;

/* Function pointer applied to a node's content */
typedef void (*node_func_t)(void *);

list_t *list_init(list_t *list);
node_t *list_add(list_t *list, void *content);
void list_destroy(list_t *list, node_func_t free_func);
void list_each(list_t const *list, node_func_t action);

#endif /* LIST_H */
