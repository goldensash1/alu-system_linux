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
 * Trial division with a mod-6 wheel: 2 and 3 are divided out first, then only
 * candidates of the form 6k +/- 1 are tested. The bound d * d <= n avoids a
 * division per iteration; d is capped at 2^32 - 1 (>= sqrt of any uint64) so
 * d * d cannot overflow. This keeps large semiprime inputs fast enough to run
 * many of them in parallel without timing out.
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
	if (n > 1)
	{
		while (n % 2 == 0)
		{
			add_factor(list, 2);
			n /= 2;
		}
		while (n % 3 == 0)
		{
			add_factor(list, 3);
			n /= 3;
		}
		for (d = 5; d <= 0xffffffffUL && d * d <= n; d += 6)
		{
			while (n % d == 0)
			{
				add_factor(list, d);
				n /= d;
			}
			while (n % (d + 2) == 0)
			{
				add_factor(list, d + 2);
				n /= d + 2;
			}
		}
		if (n > 1)
			add_factor(list, n);
	}
	return (list);
}

