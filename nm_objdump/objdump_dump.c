#include "hobjdump.h"

/**
 * ob_addr_width - Compute the width of the address column
 * @v: Highest address that will be printed for the section
 *
 * Return: The number of hexadecimal digits to use (at least 4)
 */
int ob_addr_width(uint64_t v)
{
	int w = 0;

	while (v)
	{
		w++;
		v >>= 4;
	}
	if (w < 4)
		w = 4;
	return (w);
}

/**
 * ob_print_line - Print one line of a hexadecimal section dump
 * @f: ELF file
 * @foff: File offset of the first byte on the line
 * @vaddr: Virtual address of the first byte on the line
 * @rem: Number of bytes remaining in the section
 * @w: Width of the address column
 */
void ob_print_line(elf_file *f, size_t foff, uint64_t vaddr, size_t rem, int w)
{
	int k, n = rem < 16 ? (int)rem : 16, c;

	printf(" %0*lx", w, (unsigned long)vaddr);
	for (k = 0; k < 16; k++)
	{
		if (k % 4 == 0)
			putchar(' ');
		if (k < n)
			printf("%02x", f->data[foff + k]);
		else
			printf("  ");
	}
	printf("  ");
	for (k = 0; k < 16; k++)
	{
		if (k >= n)
			putchar(' ');
		else
		{
			c = f->data[foff + k];
			putchar(isprint(c) ? c : '.');
		}
	}
	putchar('\n');
}

/**
 * ob_dump_one - Print the full hexadecimal contents of one section
 * @f: ELF file
 * @i: Section index
 */
void ob_dump_one(elf_file *f, size_t i)
{
	uint64_t addr = get_sh_addr(f, i);
	size_t off = (size_t)get_sh_offset(f, i);
	size_t size = (size_t)get_sh_size(f, i);
	size_t j;
	int w = ob_addr_width(addr + size);

	printf("Contents of section %s:\n", get_sh_str(f, i));
	for (j = 0; j < size; j += 16)
		ob_print_line(f, off + j, addr + j, size - j, w);
}
