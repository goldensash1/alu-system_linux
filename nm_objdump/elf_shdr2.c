#include "elf_common.h"

/**
 * shdr_base - Compute the file offset of a section header
 * @f: ELF file
 * @i: Section index
 *
 * Return: Byte offset of section header @i within the file
 */
size_t shdr_base(elf_file *f, size_t i)
{
	return ((size_t)get_e_shoff(f) + i * (size_t)get_e_shentsize(f));
}

/**
 * get_sh_size - Read the size of a section
 * @f: ELF file
 * @i: Section index
 *
 * Return: The sh_size field value
 */
uint64_t get_sh_size(elf_file *f, size_t i)
{
	if (f->is64)
		return (rd64(f, shdr_base(f, i) + offsetof(Elf64_Shdr, sh_size)));
	return (rd32(f, shdr_base(f, i) + offsetof(Elf32_Shdr, sh_size)));
}

/**
 * get_sh_link - Read the link field of a section
 * @f: ELF file
 * @i: Section index
 *
 * Return: The sh_link field value
 */
uint64_t get_sh_link(elf_file *f, size_t i)
{
	if (f->is64)
		return (rd32(f, shdr_base(f, i) + offsetof(Elf64_Shdr, sh_link)));
	return (rd32(f, shdr_base(f, i) + offsetof(Elf32_Shdr, sh_link)));
}

/**
 * get_sh_entsize - Read the entry size of a section
 * @f: ELF file
 * @i: Section index
 *
 * Return: The sh_entsize field value
 */
uint64_t get_sh_entsize(elf_file *f, size_t i)
{
	if (f->is64)
		return (rd64(f, shdr_base(f, i) + offsetof(Elf64_Shdr, sh_entsize)));
	return (rd32(f, shdr_base(f, i) + offsetof(Elf32_Shdr, sh_entsize)));
}

/**
 * get_sh_str - Get the name of a section as a C string
 * @f: ELF file
 * @i: Section index
 *
 * Return: Pointer to the section name inside the file buffer
 */
const char *get_sh_str(elf_file *f, size_t i)
{
	size_t stroff = (size_t)get_sh_offset(f, (size_t)get_e_shstrndx(f));

	return ((const char *)(f->data + stroff + get_sh_name(f, i)));
}
