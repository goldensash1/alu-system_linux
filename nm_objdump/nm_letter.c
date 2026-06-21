#include "hnm.h"

/**
 * nm_section_letter - Pick the type letter from a section's properties
 * @f: ELF file
 * @shndx: Section index the symbol points to
 *
 * Return: The uppercase type letter for the section
 */
char nm_section_letter(elf_file *f, uint16_t shndx)
{
	uint64_t t = get_sh_type(f, shndx);
	uint64_t fl = get_sh_flags(f, shndx);

	if (t == SHT_NOBITS && (fl & SHF_ALLOC) && (fl & SHF_WRITE))
		return ('B');
	if (t == SHT_INIT_ARRAY || t == SHT_FINI_ARRAY ||
		t == SHT_PREINIT_ARRAY)
		return ('T');
	if ((fl & SHF_ALLOC) && (fl & SHF_EXECINSTR))
		return ('T');
	if ((fl & SHF_ALLOC) && (fl & SHF_WRITE))
		return ('D');
	if (fl & SHF_ALLOC)
		return ('R');
	return ('N');
}

/**
 * nm_letter - Determine the nm type letter for a symbol
 * @f: ELF file
 * @s: Normalized symbol
 *
 * Return: The single character describing the symbol
 */
char nm_letter(elf_file *f, nsym *s)
{
	int bind = s->info >> 4, type = s->info & 0xf;
	char c;

	if (bind == STB_GNU_UNIQUE)
		return ('u');
	if (bind == STB_WEAK)
	{
		if (type == STT_OBJECT)
			return (s->shndx == SHN_UNDEF ? 'v' : 'V');
		return (s->shndx == SHN_UNDEF ? 'w' : 'W');
	}
	if (s->shndx == SHN_UNDEF)
		return ('U');
	if (s->shndx == SHN_ABS)
		c = 'A';
	else if (s->shndx == SHN_COMMON)
		c = 'C';
	else
		c = nm_section_letter(f, s->shndx);
	if (bind == STB_LOCAL)
		c = (char)tolower((unsigned char)c);
	return (c);
}
