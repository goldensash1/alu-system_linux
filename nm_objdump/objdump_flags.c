#include "hobjdump.h"

/**
 * ob_has_debug - Check whether the file carries debug sections
 * @f: ELF file
 *
 * Return: 1 if a section name starts with ".debug", 0 otherwise
 */
int ob_has_debug(elf_file *f)
{
	size_t i, n = (size_t)get_e_shnum(f);

	for (i = 1; i < n; i++)
		if (strncmp(get_sh_str(f, i), ".debug", 6) == 0)
			return (1);
	return (0);
}

/**
 * ob_compute_flags - Compute the BFD-style file flags
 * @f: ELF file
 *
 * Return: The combined flag bits as displayed by objdump
 */
uint32_t ob_compute_flags(elf_file *f)
{
	uint32_t fl = 0;
	uint64_t type = get_e_type(f);

	if (type == ET_EXEC)
		fl |= 0x002;
	if (type == ET_DYN)
		fl |= 0x040;
	if (type == ET_EXEC || type == ET_DYN)
		fl |= 0x100;
	if (find_section_by_type(f, SHT_SYMTAB) || find_section_by_type(f, SHT_DYNSYM))
		fl |= 0x010;
	if (type == ET_REL && (find_section_by_type(f, SHT_RELA) ||
		find_section_by_type(f, SHT_REL)))
		fl |= 0x001;
	if (ob_has_debug(f))
		fl |= 0x008;
	return (fl);
}

/**
 * ob_print_flags - Print the active flag names separated by commas
 * @fl: Flag bits to print
 */
void ob_print_flags(uint32_t fl)
{
	static const char * const names[] = {
		"HAS_RELOC", "EXEC_P", "HAS_LINENO", "HAS_DEBUG", "HAS_SYMS",
		"HAS_LOCALS", "DYNAMIC", "WP_TEXT", "D_PAGED"};
	static const uint32_t bits[] = {
		0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80, 0x100};
	int i, first = 1;

	for (i = 0; i < 9; i++)
		if (fl & bits[i])
		{
			if (!first)
				printf(", ");
			printf("%s", names[i]);
			first = 0;
		}
	printf("\n");
}
