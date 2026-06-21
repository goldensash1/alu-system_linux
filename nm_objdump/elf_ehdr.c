#include "elf_common.h"

/**
 * get_e_type - Read the object file type from the ELF header
 * @f: ELF file
 *
 * Return: The e_type field value
 */
uint64_t get_e_type(elf_file *f)
{
	return (rd16(f, offsetof(Elf64_Ehdr, e_type)));
}

/**
 * get_e_machine - Read the target architecture from the ELF header
 * @f: ELF file
 *
 * Return: The e_machine field value
 */
uint64_t get_e_machine(elf_file *f)
{
	return (rd16(f, offsetof(Elf64_Ehdr, e_machine)));
}

/**
 * get_e_entry - Read the program entry point address
 * @f: ELF file
 *
 * Return: The e_entry field value
 */
uint64_t get_e_entry(elf_file *f)
{
	if (f->is64)
		return (rd64(f, offsetof(Elf64_Ehdr, e_entry)));
	return (rd32(f, offsetof(Elf32_Ehdr, e_entry)));
}

/**
 * get_e_shnum - Read the number of section headers
 * @f: ELF file
 *
 * Return: The e_shnum field value
 */
uint64_t get_e_shnum(elf_file *f)
{
	if (f->is64)
		return (rd16(f, offsetof(Elf64_Ehdr, e_shnum)));
	return (rd16(f, offsetof(Elf32_Ehdr, e_shnum)));
}

/**
 * get_e_shstrndx - Read the section header string table index
 * @f: ELF file
 *
 * Return: The e_shstrndx field value
 */
uint64_t get_e_shstrndx(elf_file *f)
{
	if (f->is64)
		return (rd16(f, offsetof(Elf64_Ehdr, e_shstrndx)));
	return (rd16(f, offsetof(Elf32_Ehdr, e_shstrndx)));
}
