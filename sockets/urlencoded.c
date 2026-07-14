#include <string.h>
#include "todo_api.h"

/**
 * parse_pairs - splits a URL-encoded "key=value&key=value" string into
 * an array of key/value pairs
 * @str: the string to parse
 * @pairs: the array of pairs to fill in
 * @count: pointer to the number of pairs found, set by this function
 * @max: maximum number of pairs that pairs can hold
 */
void parse_pairs(const char *str, pair_t *pairs, int *count, int max)
{
	char copy[BUF_SIZE];
	char *token;
	char *equal_sign;
	char *saveptr;

	*count = 0;
	if (!str || !str[0])
		return;

	strncpy(copy, str, sizeof(copy) - 1);
	copy[sizeof(copy) - 1] = '\0';

	token = strtok_r(copy, "&", &saveptr);
	while (token && *count < max)
	{
		equal_sign = strchr(token, '=');
		if (equal_sign)
		{
			*equal_sign = '\0';
			strncpy(pairs[*count].key, token,
				sizeof(pairs[*count].key) - 1);
			strncpy(pairs[*count].value, equal_sign + 1,
				sizeof(pairs[*count].value) - 1);
		}
		else
		{
			strncpy(pairs[*count].key, token,
				sizeof(pairs[*count].key) - 1);
			pairs[*count].value[0] = '\0';
		}
		(*count)++;
		token = strtok_r(NULL, "&", &saveptr);
	}
}
