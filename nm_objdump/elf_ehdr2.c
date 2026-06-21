#include "elf_common.h"

/**
 * get_e_shoff - Read the section header table file offset
 * @f: ELF file
 *
 * Return: The e_shoff field value
 */
uint64_t get_e_shoff(elf_file *f)
{
	if (f->is64)
		return (rd64(f, offsetof(Elf64_Ehdr, e_shoff)));
	return (rd32(f, offsetof(Elf32_Ehdr, e_shoff)));
}

/**
 * get_e_shentsize - Read the size of one section header
 * @f: ELF file
 *
 * Return: The e_shentsize field value
 */
uint64_t get_e_shentsize(elf_file *f)
{
	if (f->is64)
		return (rd16(f, offsetof(Elf64_Ehdr, e_shentsize)));
	return (rd16(f, offsetof(Elf32_Ehdr, e_shentsize)));
}

/**
 * get_e_phoff - Read the program header table file offset
 * @f: ELF file
 *
 * Return: The e_phoff field value
 */
uint64_t get_e_phoff(elf_file *f)
{
	if (f->is64)
		return (rd64(f, offsetof(Elf64_Ehdr, e_phoff)));
	return (rd32(f, offsetof(Elf32_Ehdr, e_phoff)));
}

/**
 * get_e_phnum - Read the number of program headers
 * @f: ELF file
 *
 * Return: The e_phnum field value
 */
uint64_t get_e_phnum(elf_file *f)
{
	if (f->is64)
		return (rd16(f, offsetof(Elf64_Ehdr, e_phnum)));
	return (rd16(f, offsetof(Elf32_Ehdr, e_phnum)));
}

/**
 * get_e_phentsize - Read the size of one program header
 * @f: ELF file
 *
 * Return: The e_phentsize field value
 */
uint64_t get_e_phentsize(elf_file *f)
{
	if (f->is64)
		return (rd16(f, offsetof(Elf64_Ehdr, e_phentsize)));
	return (rd16(f, offsetof(Elf32_Ehdr, e_phentsize)));
}
