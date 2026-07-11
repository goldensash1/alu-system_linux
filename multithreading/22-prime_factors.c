#include <stdlib.h>
#include "multithreading.h"

/**
 * create_task - Allocate and initialize an executable task
 *
 * @entry: Pointer to the entry function of the task
 * @param: Parameter that will be passed to the entry function
 *
 * Return: Pointer to the created task, or NULL on failure
 */
task_t *create_task(task_entry_t entry, void *param)
{
	task_t *task;

	task = malloc(sizeof(*task));
	if (!task)
		return (NULL);
	task->entry = entry;
	task->param = param;
	task->status = PENDING;
	task->result = NULL;
	pthread_mutex_init(&task->lock, NULL);
	return (task);
}

/**
 * destroy_task - Free a task and everything it owns
 *
 * @task: Pointer to the task to destroy
 */
void destroy_task(task_t *task)
{
	if (!task)
		return;
	pthread_mutex_destroy(&task->lock);
	if (task->result)
	{
		list_destroy(task->result, free);
		free(task->result);
	}
	free(task);
}

/**
 * claim_task - Atomically claim a task if it is still pending
 *
 * @task: Pointer to the task to claim
 *
 * Return: 1 if the calling thread claimed the task, 0 otherwise
 */
static int claim_task(task_t *task)
{
	int claimed = 0;

	pthread_mutex_lock(&task->lock);
	if (task->status == PENDING)
	{
		task->status = STARTED;
		claimed = 1;
	}
	pthread_mutex_unlock(&task->lock);
	return (claimed);
}

/**
 * exec_tasks - Thread entry, executes pending tasks from a shared list
 *
 * @tasks: Pointer to the list of tasks to be executed
 *
 * Each task is guarded by its own mutex so that, even though several
 * threads walk the same list, a given task is executed exactly once.
 *
 * Return: NULL
 */
void *exec_tasks(list_t const *tasks)
{
	node_t *node;
	task_t *task;
	size_t i = 0;

	if (!tasks)
		return (NULL);
	for (node = tasks->head; node; node = node->next, i++)
	{
		task = (task_t *)node->content;
		if (!claim_task(task))
			continue;
		tprintf("[%02lu] Started\n", i);
		task->result = task->entry(task->param);
		pthread_mutex_lock(&task->lock);
		task->status = task->result ? SUCCESS : FAILURE;
		pthread_mutex_unlock(&task->lock);
		tprintf("[%02lu] %s\n", i,
			task->result ? "Success" : "Failure");
	}
	return (NULL);
}
