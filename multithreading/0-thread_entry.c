#include <stdio.h>
#include <pthread.h>
#include "multithreading.h"

/**
 * thread_entry - Entry point of a new thread
 *
 * @arg: Address of a string to print, followed by a new line
 *
 * Return: NULL (the calling thread is terminated before returning)
 */
void *thread_entry(void *arg)
{
	char *str;

	str = (char *)arg;
	printf("%s\n", str);
	pthread_exit(NULL);
	return (NULL);
}
