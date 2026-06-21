#ifndef ELF_COMMON_H
#define ELF_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <ctype.h>
#include <errno.h>
#include <sys/stat.h>
#include <elf.h>

/**
 * struct elf_file - In-memory representation of an ELF object file
 * @data: Pointer to the raw bytes of the whole file
 * @size: Size of the file in bytes
 * @name: Path of the file as given on the command line
 * @is64: Non-zero when the file uses the 64-bit ELF class
 * @be: Non-zero when the file stores data in big-endian byte order
 * @swap: Non-zero when the file endianness differs from the host
 */
typedef struct elf_file
{
	unsigned char *data;
	size_t size;
	const char *name;
	int is64;
	int be;
	int swap;
} elf_file;

/* endian.c */
uint16_t swap16(uint16_t v);
uint32_t swap32(uint32_t v);
uint64_t swap64(uint64_t v);
int host_be(void);

/* read_field.c */
uint16_t rd16(elf_file *f, size_t off);
uint32_t rd32(elf_file *f, size_t off);
uint64_t rd64(elf_file *f, size_t off);

/* read_elf.c */
int read_file(const char *path, elf_file *f);
int check_elf(elf_file *f);
void free_file(elf_file *f);
size_t find_section_by_type(elf_file *f, uint32_t type);

/* elf_ehdr.c */
uint64_t get_e_type(elf_file *f);
uint64_t get_e_machine(elf_file *f);
uint64_t get_e_entry(elf_file *f);
uint64_t get_e_shnum(elf_file *f);
uint64_t get_e_shstrndx(elf_file *f);

/* elf_ehdr2.c */
uint64_t get_e_shoff(elf_file *f);
uint64_t get_e_shentsize(elf_file *f);
uint64_t get_e_phoff(elf_file *f);
uint64_t get_e_phnum(elf_file *f);
uint64_t get_e_phentsize(elf_file *f);

/* elf_shdr.c */
size_t shdr_base(elf_file *f, size_t i);
uint64_t get_sh_name(elf_file *f, size_t i);
uint64_t get_sh_type(elf_file *f, size_t i);
uint64_t get_sh_flags(elf_file *f, size_t i);
uint64_t get_sh_addr(elf_file *f, size_t i);
uint64_t get_sh_offset(elf_file *f, size_t i);

/* elf_shdr2.c */
uint64_t get_sh_size(elf_file *f, size_t i);
uint64_t get_sh_link(elf_file *f, size_t i);
uint64_t get_sh_entsize(elf_file *f, size_t i);
const char *get_sh_str(elf_file *f, size_t i);

#endif /* ELF_COMMON_H */
