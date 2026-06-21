#include "elf_common.h"

/**
 * read_file - Load the whole content of a file into memory
 * @path: Path of the file to read
 * @f: ELF file structure to populate
 *
 * Return: 0 on success, 1 on failure (errno is set)
 */
int read_file(const char *path, elf_file *f)
{
	int fd;
	ssize_t got;
	struct stat st;

	memset(f, 0, sizeof(*f));
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (1);
	if (fstat(fd, &st) < 0 || !S_ISREG(st.st_mode))
	{
		close(fd);
		errno = S_ISDIR(st.st_mode) ? EISDIR : errno;
		return (1);
	}
	f->size = (size_t)st.st_size;
	f->data = malloc(f->size ? f->size : 1);
	if (!f->data)
	{
		close(fd);
		return (1);
	}
	got = read(fd, f->data, f->size);
	close(fd);
	if (got != (ssize_t)f->size)
	{
		free(f->data);
		f->data = NULL;
		return (1);
	}
	f->name = path;
	return (0);
}

/**
 * check_elf - Validate the ELF magic and read class and endianness
 * @f: ELF file to inspect
 *
 * Return: 0 if the file is a supported ELF object, 1 otherwise
 */
int check_elf(elf_file *f)
{
	unsigned char *d = f->data;

	if (f->size < EI_NIDENT || memcmp(d, ELFMAG, SELFMAG) != 0)
		return (1);
	if (d[EI_CLASS] == ELFCLASS64)
		f->is64 = 1;
	else if (d[EI_CLASS] == ELFCLASS32)
		f->is64 = 0;
	else
		return (1);
	if (d[EI_DATA] == ELFDATA2MSB)
		f->be = 1;
	else if (d[EI_DATA] == ELFDATA2LSB)
		f->be = 0;
	else
		return (1);
	f->swap = (f->be != host_be());
	return (0);
}

/**
 * free_file - Release the memory held by an ELF file
 * @f: ELF file to free
 */
void free_file(elf_file *f)
{
	if (f->data)
		free(f->data);
	f->data = NULL;
}

/**
 * find_section_by_type - Find the first section of a given type
 * @f: ELF file to search
 * @type: Section type to look for (SHT_*)
 *
 * Return: Index of the matching section, or 0 if none is found
 */
size_t find_section_by_type(elf_file *f, uint32_t type)
{
	size_t i, n = (size_t)get_e_shnum(f);

	for (i = 1; i < n; i++)
		if ((uint32_t)get_sh_type(f, i) == type)
			return (i);
	return (0);
}
