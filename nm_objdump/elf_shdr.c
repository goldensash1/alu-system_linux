#include "elf_common.h"

/**
 * get_sh_name - Read the name offset of a section
 * @f: ELF file
 * @i: Section index
 *
 * Return: The sh_name field value
 */
uint64_t get_sh_name(elf_file *f, size_t i)
{
	return (rd32(f, shdr_base(f, i) + offsetof(Elf64_Shdr, sh_name)));
}

/**
 * get_sh_type - Read the type of a section
 * @f: ELF file
 * @i: Section index
 *
 * Return: The sh_type field value
 */
uint64_t get_sh_type(elf_file *f, size_t i)
{
	return (rd32(f, shdr_base(f, i) + offsetof(Elf64_Shdr, sh_type)));
}

/**
 * get_sh_flags - Read the flags of a section
 * @f: ELF file
 * @i: Section index
 *
 * Return: The sh_flags field value
 */
uint64_t get_sh_flags(elf_file *f, size_t i)
{
	if (f->is64)
		return (rd64(f, shdr_base(f, i) + offsetof(Elf64_Shdr, sh_flags)));
	return (rd32(f, shdr_base(f, i) + offsetof(Elf32_Shdr, sh_flags)));
}

/**
 * get_sh_addr - Read the virtual address of a section
 * @f: ELF file
 * @i: Section index
 *
 * Return: The sh_addr field value
 */
uint64_t get_sh_addr(elf_file *f, size_t i)
{
	if (f->is64)
		return (rd64(f, shdr_base(f, i) + offsetof(Elf64_Shdr, sh_addr)));
	return (rd32(f, shdr_base(f, i) + offsetof(Elf32_Shdr, sh_addr)));
}

/**
 * get_sh_offset - Read the file offset of a section
 * @f: ELF file
 * @i: Section index
 *
 * Return: The sh_offset field value
 */
uint64_t get_sh_offset(elf_file *f, size_t i)
{
	if (f->is64)
		return (rd64(f, shdr_base(f, i) + offsetof(Elf64_Shdr, sh_offset)));
	return (rd32(f, shdr_base(f, i) + offsetof(Elf32_Shdr, sh_offset)));
}
