#include "hnm.h"

/**
 * ver_fill - Resolve the version suffix of one dynamic symbol
 * @f: ELF file
 * @vm: Version map being built
 * @c: Version resolution context
 * @i: Index of the dynamic symbol
 */
static void ver_fill(elf_file *f, vmap *vm, vctx *c, size_t i)
{
	uint32_t nameoff = rd32(f, c->base + i * c->entsz);
	const char *vstr, *sep;
	uint16_t v;
	size_t len;

	vm->names[i] = (char *)(f->data + c->dynstr + nameoff);
	vm->suffix[i] = NULL;
	if (!c->versym)
		return;
	v = rd16(f, c->versym + i * 2);
	sep = (v & 0x8000) ? "@" : "@@";
	v &= 0x7fff;
	if (v < 2)
		return;
	vstr = ver_index_string(f, c, v);
	if (!vstr)
		return;
	len = strlen(sep) + strlen(vstr) + 1;
	vm->suffix[i] = malloc(len);
	if (vm->suffix[i])
	{
		strcpy(vm->suffix[i], sep);
		strcat(vm->suffix[i], vstr);
	}
}

/**
 * sec_off - Get the file offset of the first section of a given type
 * @f: ELF file
 * @type: Section type to look for (SHT_*)
 *
 * Return: File offset of the section, or 0 when it is absent
 */
static size_t sec_off(elf_file *f, uint32_t type)
{
	return ((size_t)get_sh_offset(f, find_section_by_type(f, type)));
}

/**
 * ver_build - Build the dynamic-symbol version map for a file
 * @f: ELF file
 * @vm: Version map to populate
 */
void ver_build(elf_file *f, vmap *vm)
{
	size_t ds = find_section_by_type(f, SHT_DYNSYM), i;
	unsigned char osabi = f->data[EI_OSABI];
	vctx c;

	vm->n = 0;
	vm->names = NULL;
	vm->suffix = NULL;
	if (osabi != ELFOSABI_NONE && osabi != ELFOSABI_GNU)
		return;
	if (!ds)
		return;
	c.dynstr = (size_t)get_sh_offset(f, (size_t)get_sh_link(f, ds));
	c.versym = sec_off(f, SHT_GNU_versym);
	c.verneed = sec_off(f, SHT_GNU_verneed);
	c.verdef = sec_off(f, SHT_GNU_verdef);
	c.base = (size_t)get_sh_offset(f, ds);
	c.entsz = (size_t)get_sh_entsize(f, ds);
	vm->n = c.entsz ? (size_t)get_sh_size(f, ds) / c.entsz : 0;
	vm->names = calloc(vm->n ? vm->n : 1, sizeof(char *));
	vm->suffix = calloc(vm->n ? vm->n : 1, sizeof(char *));
	if (!vm->names || !vm->suffix)
	{
		vm->n = 0;
		return;
	}
	for (i = 0; i < vm->n; i++)
		ver_fill(f, vm, &c, i);
}

/**
 * ver_lookup - Find the version suffix for a symbol name
 * @vm: Version map
 * @name: Symbol name to look up
 *
 * Return: The version suffix, or an empty string when none applies
 */
const char *ver_lookup(vmap *vm, const char *name)
{
	size_t i;

	for (i = 0; i < vm->n; i++)
		if (vm->names[i] && vm->suffix[i] && !strcmp(vm->names[i], name))
			return (vm->suffix[i]);
	return ("");
}

/**
 * ver_free - Release the memory held by a version map
 * @vm: Version map to free
 */
void ver_free(vmap *vm)
{
	size_t i;

	for (i = 0; i < vm->n; i++)
		free(vm->suffix[i]);
	free(vm->names);
	free(vm->suffix);
}
