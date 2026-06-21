#ifndef HOBJDUMP_H
#define HOBJDUMP_H

#include "elf_common.h"

/* hobjdump.c */
int hobjdump_file(const char *path);

/* objdump_header.c */
void ob_print_header(elf_file *f);
void ob_file(elf_file *f);

/* objdump_flags.c */
uint32_t ob_compute_flags(elf_file *f);
void ob_print_flags(uint32_t fl);
int ob_has_debug(elf_file *f);

/* objdump_arch.c */
const char *ob_format_name(elf_file *f);
const char *ob_arch_name(elf_file *f);

/* objdump_sections.c */
void ob_dump_sections(elf_file *f);

/* objdump_dump.c */
int ob_addr_width(uint64_t v);
void ob_print_line(elf_file *f, size_t foff, uint64_t vaddr, size_t rem, int w);
void ob_dump_one(elf_file *f, size_t i);

#endif /* HOBJDUMP_H */
