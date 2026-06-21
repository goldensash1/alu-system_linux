CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic

COMMON = endian.c read_field.c read_elf.c elf_ehdr.c elf_ehdr2.c \
	elf_shdr.c elf_shdr2.c

NM_SRC = hnm.c nm_table.c nm_letter.c nm_print.c nm_version.c \
	nm_verscan.c $(COMMON)

OBJ_SRC = hobjdump.c objdump_header.c objdump_flags.c objdump_arch.c \
	objdump_sections.c objdump_dump.c $(COMMON)

all: hnm hobjdump

hnm: $(NM_SRC)
	$(CC) $(CFLAGS) $(NM_SRC) -o hnm

hobjdump: $(OBJ_SRC)
	$(CC) $(CFLAGS) $(OBJ_SRC) -o hobjdump

clean:
	$(RM) hnm hobjdump

re: clean all

.PHONY: all clean re
