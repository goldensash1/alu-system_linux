#include "hobjdump.h"

/**
 * ob_skip - Decide whether a section must be skipped by "objdump -s"
 * @f: ELF file
 * @i: Section index
 * @shstr: Index of the section header string table
 * @symstr: Index of the string table linked to the symbol table
 *
 * Return: 1 if the section should not be dumped, 0 otherwise
 */
static int ob_skip(elf_file *f, size_t i, size_t shstr, size_t symstr)
{
	uint64_t t = get_sh_type(f, i);

	if (i == shstr || i == symstr)
		return (1);
	if (t == SHT_SYMTAB || t == SHT_SYMTAB_SHNDX)
		return (1);
	if (t == SHT_NULL || t == SHT_NOBITS)
		return (1);
	if (get_sh_size(f, i) == 0)
		return (1);
	return (0);
}

/**
 * ob_dump_sections - Dump the contents of every relevant section
 * @f: ELF file
 */
void ob_dump_sections(elf_file *f)
{
	size_t i, n = (size_t)get_e_shnum(f);
	size_t shstr = (size_t)get_e_shstrndx(f);
	size_t st = find_section_by_type(f, SHT_SYMTAB);
	size_t symstr = st ? (size_t)get_sh_link(f, st) : 0;

	printf("\n");
	for (i = 1; i < n; i++)
		if (!ob_skip(f, i, shstr, symstr))
			ob_dump_one(f, i);
}
