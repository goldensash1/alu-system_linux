#include <stdlib.h>
#include "multithreading.h"

/**
 * add_factor - Allocate a prime factor and append it to a list
 *
 * @list:   List to append the factor to
 * @factor: Value of the prime factor to store
 */
static void add_factor(list_t *list, unsigned long factor)
{
	unsigned long *value;

	value = malloc(sizeof(*value));
	if (!value)
		return;
	*value = factor;
	list_add(list, value);
}

/**
 * prime_factors - Factorize a number into its list of prime factors
 *
 * @s: String representation of a positive number fitting an unsigned long
 *
 * Return: Pointer to a newly allocated list of prime factors, or NULL
 */
list_t *prime_factors(char const *s)
{
	list_t *list;
	unsigned long n, d;

	list = malloc(sizeof(*list));
	if (!list)
		return (NULL);
	list_init(list);
	n = strtoul(s, NULL, 10);
	while (n % 2 == 0 && n > 0)
	{
		add_factor(list, 2);
		n /= 2;
	}
	for (d = 3; d <= n / d; d += 2)
	{
		while (n % d == 0)
		{
			add_factor(list, d);
			n /= d;
		}
	}
	if (n > 1)
		add_factor(list, n);
	return (list);
}
