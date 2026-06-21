#include "hobjdump.h"

/**
 * hobjdump_file - Process a single file for the hobjdump command
 * @path: Path of the file to analyze
 *
 * Return: 0 on success, 1 on any error
 */
int hobjdump_file(const char *path)
{
	elf_file f;

	if (read_file(path, &f))
	{
		fprintf(stderr, "hobjdump: '%s': %s\n", path, strerror(errno));
		return (1);
	}
	if (check_elf(&f))
	{
		fprintf(stderr, "hobjdump: %s: File format not recognized\n", path);
		free_file(&f);
		return (1);
	}
	f.name = path;
	ob_file(&f);
	free_file(&f);
	return (0);
}

/**
 * main - Entry point for the hobjdump program
 * @argc: Argument count
 * @argv: Argument vector
 *
 * Return: 0 on success, 1 if any file could not be processed
 */
int main(int argc, char **argv)
{
	int i, n = argc - 1, status = 0;
	char *def[] = {"a.out"};
	char **files = argv + 1;

	if (n == 0)
	{
		files = def;
		n = 1;
	}
	for (i = 0; i < n; i++)
		status |= hobjdump_file(files[i]);
	return (status);
}
