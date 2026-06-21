#ifndef HNM_H
#define HNM_H

#include "elf_common.h"

/**
 * struct nsym - Normalized ELF symbol independent of the ELF class
 * @name: Offset of the symbol name in its string table
 * @value: Symbol value (address)
 * @info: Symbol type and binding information
 * @other: Symbol visibility
 * @shndx: Section index the symbol belongs to
 */
typedef struct nsym
{
	uint32_t name;
	uint64_t value;
	unsigned char info;
	unsigned char other;
	uint16_t shndx;
} nsym;

/**
 * struct vmap - Mapping of dynamic symbol names to version suffixes
 * @n: Number of entries
 * @names: Array of symbol names (pointers into the dynstr table)
 * @suffix: Array of allocated version suffix strings ("@@ver" or NULL)
 */
typedef struct vmap
{
	size_t n;
	char **names;
	char **suffix;
} vmap;

/**
 * struct vctx - Offsets needed to resolve symbol versions
 * @dynstr: File offset of the dynamic string table
 * @versym: File offset of the .gnu.version section (0 if absent)
 * @verneed: File offset of the .gnu.version_r section (0 if absent)
 * @verdef: File offset of the .gnu.version_d section (0 if absent)
 * @base: File offset of the dynamic symbol table
 * @entsz: Size of one dynamic symbol entry
 */
typedef struct vctx
{
	size_t dynstr;
	size_t versym;
	size_t verneed;
	size_t verdef;
	size_t base;
	size_t entsz;
} vctx;

/* hnm.c */
int hnm_file(const char *prog, const char *path, int multi, int idx);

/* nm_table.c */
nsym read_sym(elf_file *f, size_t base, size_t entsz, size_t i);
int nm_process(elf_file *f, const char *prog);

/* nm_letter.c */
char nm_section_letter(elf_file *f, uint16_t shndx);
char nm_letter(elf_file *f, nsym *s);

/* nm_print.c */
void nm_print_symbol(elf_file *f, nsym *s, const char *name, const char *suf);

/* nm_version.c */
void ver_build(elf_file *f, vmap *vm);
const char *ver_lookup(vmap *vm, const char *name);
void ver_free(vmap *vm);

/* nm_verscan.c */
const char *ver_index_string(elf_file *f, vctx *c, uint16_t idx);

#endif /* HNM_H */
