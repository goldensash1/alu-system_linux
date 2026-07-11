#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>
#include "multithreading.h"

/**
 * tprintf - Print a formatted string preceded by the calling thread ID
 *
 * @format: Format string passed to the printf family
 *
 * Return: Number of characters printed by the formatted output
 */
int tprintf(char const *format, ...)
{
	va_list args;
	int ret;

	va_start(args, format);
	printf("[%lu] ", (unsigned long)pthread_self());
	ret = vprintf(format, args);
	va_end(args);
	return (ret);
}
