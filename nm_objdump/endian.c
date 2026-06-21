#include "elf_common.h"

/**
 * swap16 - Reverse the byte order of a 16-bit value
 * @v: Value to swap
 *
 * Return: The byte-swapped value
 */
uint16_t swap16(uint16_t v)
{
	return ((uint16_t)((v >> 8) | (v << 8)));
}

/**
 * swap32 - Reverse the byte order of a 32-bit value
 * @v: Value to swap
 *
 * Return: The byte-swapped value
 */
uint32_t swap32(uint32_t v)
{
	return (((v & 0x000000ffU) << 24) | ((v & 0x0000ff00U) << 8) |
		((v & 0x00ff0000U) >> 8) | ((v & 0xff000000U) >> 24));
}

/**
 * swap64 - Reverse the byte order of a 64-bit value
 * @v: Value to swap
 *
 * Return: The byte-swapped value
 */
uint64_t swap64(uint64_t v)
{
	return (((uint64_t)swap32((uint32_t)v) << 32) |
		(uint64_t)swap32((uint32_t)(v >> 32)));
}

/**
 * host_be - Detect whether the host CPU is big-endian
 *
 * Return: 1 if the host is big-endian, 0 otherwise
 */
int host_be(void)
{
	uint16_t x = 1;

	return (*(unsigned char *)&x == 0);
}
