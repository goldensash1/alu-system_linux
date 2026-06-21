#include "hnm.h"

/**
 * scan_verneed - Search the version needs table for an index
 * @f: ELF file
 * @c: Version resolution context
 * @idx: Version index to resolve
 *
 * Return: The version name, or NULL when the index is not found
 */
static const char *scan_verneed(elf_file *f, vctx *c, uint16_t idx)
{
	size_t off = c->verneed, aux;
	uint16_t cnt, j;
	uint32_t next;

	if (!off)
		return (NULL);
	while (1)
	{
		cnt = rd16(f, off + offsetof(Elf64_Verneed, vn_cnt));
		aux = off + rd32(f, off + offsetof(Elf64_Verneed, vn_aux));
		for (j = 0; j < cnt; j++)
		{
			if ((rd16(f, aux + offsetof(Elf64_Vernaux, vna_other))
				& 0x7fff) == idx)
				return ((char *)(f->data + c->dynstr +
					rd32(f, aux + offsetof(Elf64_Vernaux, vna_name))));
			aux += rd32(f, aux + offsetof(Elf64_Vernaux, vna_next));
		}
		next = rd32(f, off + offsetof(Elf64_Verneed, vn_next));
		if (!next)
			break;
		off += next;
	}
	return (NULL);
}

/**
 * scan_verdef - Search the version definitions table for an index
 * @f: ELF file
 * @c: Version resolution context
 * @idx: Version index to resolve
 *
 * Return: The version name, or NULL when the index is not found
 */
static const char *scan_verdef(elf_file *f, vctx *c, uint16_t idx)
{
	size_t off = c->verdef, aux;
	uint32_t next;

	if (!off)
		return (NULL);
	while (1)
	{
		if ((rd16(f, off + offsetof(Elf64_Verdef, vd_ndx)) & 0x7fff) == idx)
		{
			aux = off + rd32(f, off + offsetof(Elf64_Verdef, vd_aux));
			return ((char *)(f->data + c->dynstr +
				rd32(f, aux + offsetof(Elf64_Verdaux, vda_name))));
		}
		next = rd32(f, off + offsetof(Elf64_Verdef, vd_next));
		if (!next)
			break;
		off += next;
	}
	return (NULL);
}

/**
 * ver_index_string - Resolve a version index to its name
 * @f: ELF file
 * @c: Version resolution context
 * @idx: Version index to resolve
 *
 * Return: The version name, or NULL when it cannot be resolved
 */
const char *ver_index_string(elf_file *f, vctx *c, uint16_t idx)
{
	const char *r = scan_verneed(f, c, idx);

	if (r)
		return (r);
	return (scan_verdef(f, c, idx));
}
