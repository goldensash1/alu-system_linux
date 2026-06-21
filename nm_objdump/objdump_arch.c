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
	if (m == EM_SPARC || m == EM_SPARC32PLUS)
		return ("elf32-sparc");
	if (m == EM_SPARCV9)
		return ("elf64-sparc");
	if (m == EM_PPC)
		return ("elf32-powerpc");
	if (m == EM_PPC64)
		return (f->be ? "elf64-powerpc" : "elf64-powerpcle");
	if (m == EM_ARM)
		return (f->be ? "elf32-bigarm" : "elf32-littlearm");
	if (m == EM_AARCH64)
		return (f->be ? "elf64-bigaarch64" : "elf64-littleaarch64");
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
	if (m == EM_SPARC || m == EM_SPARC32PLUS)
		return ("sparc");
	if (m == EM_SPARCV9)
		return ("sparc:v9");
	if (m == EM_PPC)
		return ("powerpc:common");
	if (m == EM_PPC64)
		return ("powerpc:common64");
	if (m == EM_ARM)
		return ("arm");
	if (m == EM_AARCH64)
		return ("aarch64");
	return ("UNKNOWN!");
}
