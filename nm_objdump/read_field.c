#include "elf_common.h"

/**
 * rd16 - Read a 16-bit field honoring the file endianness
 * @f: ELF file being read
 * @off: Byte offset of the field within the file
 *
 * Return: The field value in host byte order
 */
uint16_t rd16(elf_file *f, size_t off)
{
	uint16_t v;

	memcpy(&v, f->data + off, sizeof(v));
	return (f->swap ? swap16(v) : v);
}

/**
 * rd32 - Read a 32-bit field honoring the file endianness
 * @f: ELF file being read
 * @off: Byte offset of the field within the file
 *
 * Return: The field value in host byte order
 */
uint32_t rd32(elf_file *f, size_t off)
{
	uint32_t v;

	memcpy(&v, f->data + off, sizeof(v));
	return (f->swap ? swap32(v) : v);
}

/**
 * rd64 - Read a 64-bit field honoring the file endianness
 * @f: ELF file being read
 * @off: Byte offset of the field within the file
 *
 * Return: The field value in host byte order
 */
uint64_t rd64(elf_file *f, size_t off)
{
	uint64_t v;

	memcpy(&v, f->data + off, sizeof(v));
	return (f->swap ? swap64(v) : v);
}
