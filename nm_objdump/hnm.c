#include "hnm.h"

/**
 * hnm_file - Process a single file for the hnm command
 * @path: Path of the file to analyze
 * @multi: Non-zero when several files were given on the command line
 * @idx: Index of this file among the command line arguments
 *
 * Return: 0 on success, 1 on any error
 */
int hnm_file(const char *path, int multi, int idx)
{
	elf_file f;
	int r;

	if (read_file(path, &f))
	{
		fprintf(stderr, "hnm: '%s': %s\n", path, strerror(errno));
		return (1);
	}
	if (check_elf(&f))
	{
		fprintf(stderr, "hnm: %s: file format not recognized\n", path);
		free_file(&f);
		return (1);
	}
	f.name = path;
	if (multi)
	{
		if (idx)
			printf("\n");
		printf("%s:\n", path);
	}
	r = nm_process(&f);
	free_file(&f);
	return (r);
}

/**
 * main - Entry point for the hnm program
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
		status |= hnm_file(files[i], n > 1, i);
	return (status);
}
