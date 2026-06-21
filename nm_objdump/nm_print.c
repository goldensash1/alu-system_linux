#include "hnm.h"

/**
 * nm_print_symbol - Print one symbol the way "nm -p" does
 * @f: ELF file
 * @s: Normalized symbol
 * @name: Symbol name
 * @suf: Version suffix to append, or an empty string
 */
void nm_print_symbol(elf_file *f, nsym *s, const char *name, const char *suf)
{
	char c = nm_letter(f, s);
	int w = f->is64 ? 16 : 8;

	if (s->shndx == SHN_UNDEF)
		printf("%*s %c %s%s\n", w, "", c, name, suf);
	else
		printf("%0*lx %c %s%s\n", w, (unsigned long)s->value, c, name, suf);
}
