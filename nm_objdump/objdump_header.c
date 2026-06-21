#include "hobjdump.h"

/**
 * ob_print_header - Print the architecture, flags and entry point lines
 * @f: ELF file
 */
void ob_print_header(elf_file *f)
{
	uint32_t flags = ob_compute_flags(f);
	int w = f->is64 ? 16 : 8;

	printf("architecture: %s, flags 0x%08x:\n", ob_arch_name(f), flags);
	ob_print_flags(flags);
	printf("start address 0x%0*lx\n", w, (unsigned long)get_e_entry(f));
}

/**
 * ob_file - Produce the full "objdump -sf" output for a file
 * @f: ELF file
 */
void ob_file(elf_file *f)
{
	printf("\n%s:     file format %s\n", f->name, ob_format_name(f));
	ob_print_header(f);
	ob_dump_sections(f);
}
