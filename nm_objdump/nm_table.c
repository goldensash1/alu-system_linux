#include "hnm.h"

/**
 * read_sym - Read one symbol table entry into a normalized structure
 * @f: ELF file
 * @base: File offset of the symbol table
 * @entsz: Size of one symbol entry
 * @i: Index of the symbol to read
 *
 * Return: The normalized symbol
 */
nsym read_sym(elf_file *f, size_t base, size_t entsz, size_t i)
{
	size_t o = base + i * entsz;
	nsym s;

	s.name = rd32(f, o + offsetof(Elf64_Sym, st_name));
	if (f->is64)
	{
		s.info = f->data[o + offsetof(Elf64_Sym, st_info)];
		s.other = f->data[o + offsetof(Elf64_Sym, st_other)];
		s.shndx = rd16(f, o + offsetof(Elf64_Sym, st_shndx));
		s.value = rd64(f, o + offsetof(Elf64_Sym, st_value));
	}
	else
	{
		s.info = f->data[o + offsetof(Elf32_Sym, st_info)];
		s.other = f->data[o + offsetof(Elf32_Sym, st_other)];
		s.shndx = rd16(f, o + offsetof(Elf32_Sym, st_shndx));
		s.value = rd32(f, o + offsetof(Elf32_Sym, st_value));
	}
	return (s);
}

/**
 * nm_emit - Print every printable symbol of a symbol table
 * @f: ELF file
 * @sym: Index of the symbol table section
 * @vm: Version map built from the dynamic symbols
 */
static void nm_emit(elf_file *f, size_t sym, vmap *vm)
{
	size_t base = (size_t)get_sh_offset(f, sym);
	size_t entsz = (size_t)get_sh_entsize(f, sym);
	size_t strtab = (size_t)get_sh_offset(f, (size_t)get_sh_link(f, sym));
	size_t i, count = entsz ? (size_t)get_sh_size(f, sym) / entsz : 0;
	const char *name;
	nsym s;

	for (i = 1; i < count; i++)
	{
		s = read_sym(f, base, entsz, i);
		if ((s.info & 0xf) == STT_FILE || (s.info & 0xf) == STT_SECTION)
			continue;
		name = (const char *)(f->data + strtab + s.name);
		if (!*name)
			continue;
		nm_print_symbol(f, &s, name, ver_lookup(vm, name));
	}
}

/**
 * nm_process - Locate the symbol table and print all symbols
 * @f: ELF file
 *
 * Return: 0 on success, 1 when the file has no symbols
 */
int nm_process(elf_file *f)
{
	size_t sym = find_section_by_type(f, SHT_SYMTAB);
	vmap vm;

	if (!sym)
	{
		fprintf(stderr, "%s: %s: no symbols\n", prog, f->name);
		return (1);
	}
	ver_build(f, &vm);
	nm_emit(f, sym, &vm);
	ver_free(&vm);
	return (0);
}
