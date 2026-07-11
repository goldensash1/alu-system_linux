#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>
#include "multithreading.h"

void init_tprintf(void) __attribute__((constructor));
void deinit_tprintf(void) __attribute__((destructor));

static pthread_mutex_t mutex;

/**
 * init_tprintf - Initialize the tprintf mutex before main() is called
 */
void init_tprintf(void)
{
	pthread_mutex_init(&mutex, NULL);
}

/**
 * deinit_tprintf - Destroy the tprintf mutex after main() returns
 */
void deinit_tprintf(void)
{
	pthread_mutex_destroy(&mutex);
}

/**
 * tprintf - Print a formatted string preceded by the calling thread ID
 *
 * @format: Format string passed to the printf family
 *
 * A mutex serializes the whole output so concurrent threads do not
 * interleave their lines (race condition free).
 *
 * Return: Number of characters printed by the formatted output
 */
int tprintf(char const *format, ...)
{
	va_list args;
	int ret;

	va_start(args, format);
	pthread_mutex_lock(&mutex);
	printf("[%lu] ", (unsigned long)pthread_self());
	ret = vprintf(format, args);
	pthread_mutex_unlock(&mutex);
	va_end(args);
	return (ret);
}
