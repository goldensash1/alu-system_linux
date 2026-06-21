#include "hobjdump.h"

/**
 * ob_format_name - Compute the BFD target name of the file format
 * @f: ELF file
 *
 * Return: A string such as "elf64-x86-64" describing the file format
 */
const char *ob_format_name(elf_file *f)
{
	static char buf[24];
	uint64_t m = get_e_machine(f);

	if (m == EM_X86_64)
		return ("elf64-x86-64");
	if (m == EM_386)
		return ("elf32-i386");
	sprintf(buf, "elf%d-%s", f->is64 ? 64 : 32, f->be ? "big" : "little");
	return (buf);
}

/**
 * ob_arch_name - Compute the BFD printable architecture name
 * @f: ELF file
 *
 * Return: A string such as "i386:x86-64" describing the architecture
 */
const char *ob_arch_name(elf_file *f)
{
	uint64_t m = get_e_machine(f);

	if (m == EM_X86_64)
		return ("i386:x86-64");
	if (m == EM_386)
		return ("i386");
	return ("UNKNOWN!");
}
