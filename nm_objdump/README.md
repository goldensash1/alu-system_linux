# nm_objdump

Reimplementations of two GNU binary-utilities, written in C, that read and
parse [ELF](https://en.wikipedia.org/wiki/Executable_and_Linkable_Format)
object files directly using the data structures defined in
`/usr/include/elf.h`.

| Program     | Reproduces      | Usage                  |
|-------------|-----------------|------------------------|
| `hnm`       | `nm -p`         | `./hnm [objfile ...]`  |
| `hobjdump`  | `objdump -sf`   | `./hobjdump [objfile ...]` |

Both programs handle:

* **32-bit and 64-bit** ELF files (`ELFCLASS32` / `ELFCLASS64`)
* **Little-endian and big-endian** files (`ELFDATA2LSB` / `ELFDATA2MSB`),
  byte-swapping every field when the file endianness differs from the host
* All ELF object types (`ET_REL`, `ET_EXEC`, `ET_DYN`, ...)

## Build

```
make            # builds both hnm and hobjdump
make hnm        # builds hnm only
make hobjdump   # builds hobjdump only
make clean      # removes the executables
```

Compiled on Ubuntu 14.04 LTS with `gcc 4.8.4` using
`-Wall -Werror -Wextra -pedantic`.

## hnm

`hnm` lists the symbols of an object file in symbol-table order (like
`nm -p`). Each line is made of three columns:

```
<value> <type> <name>
```

* **value** – the symbol address, printed as 16 hex digits for 64-bit files
  and 8 hex digits for 32-bit files. Left blank for undefined symbols.
* **type** – a single character describing the symbol. Uppercase means a
  global (external) symbol, lowercase means a local one:

  | Letter | Meaning                                   |
  |--------|-------------------------------------------|
  | `T/t`  | code (`.text`)                            |
  | `D/d`  | initialised data (`.data`, `.dynamic`)    |
  | `B/b`  | uninitialised data (`.bss`)               |
  | `R/r`  | read-only data (`.rodata`)                |
  | `A/a`  | absolute value                            |
  | `C/c`  | common symbol                             |
  | `U`    | undefined                                 |
  | `W/w`  | weak (`V/v` for a weak object)            |
  | `u`    | unique global                             |

When a symbol carries version information (built from `.gnu.version`,
`.gnu.version_r` and `.gnu.version_d`), the version is appended, for example
`write@@GLIBC_2.2.5`.

```
./hnm elf_files/ubuntu64
```

## hobjdump

`hobjdump` reproduces `objdump -sf`: it prints the file header information
(`-f`) followed by a full hexadecimal dump of the contents of every loadable
section (`-s`).

```
<file>:     file format elf64-x86-64
architecture: i386:x86-64, flags 0x00000112:
EXEC_P, HAS_SYMS, D_PAGED
start address 0x0000000000400600

Contents of section .interp:
 400238 2f6c6962 36342f6c 642d6c69 6e75782d  /lib64/ld-linux-
 ...
```

The `.symtab`, `.strtab` and `.shstrtab` sections and any `SHT_NOBITS`
section (such as `.bss`) are not dumped, matching `objdump`'s behaviour.

## How an ELF file is parsed

The file is read entirely into memory. The 16-byte `e_ident` array at the
start gives the class (32/64-bit) and endianness, which drives every
subsequent read. Three header tables are then walked:

1. the **ELF header** (`Elf32_Ehdr` / `Elf64_Ehdr`) – locates the section
   and program header tables and the entry point;
2. the **section header table** (`Elf*_Shdr`) – describes each section
   (name, type, flags, address, offset, size);
3. the **symbol tables** (`Elf*_Sym` in `.symtab` / `.dynsym`) – the list of
   symbols, with names resolved through the linked string table.

All multi-byte fields are read through small helpers (`rd16`, `rd32`,
`rd64`) that transparently byte-swap when needed, so the same code path
serves both little- and big-endian inputs.

## File layout

| File | Role |
|------|------|
| `elf_common.h` | shared types and prototypes |
| `endian.c`, `read_field.c` | endianness detection and field readers |
| `read_elf.c` | file loading and ELF validation |
| `elf_ehdr*.c`, `elf_shdr*.c` | class-aware ELF/section header accessors |
| `hnm.c`, `nm_*.c` | the `hnm` program |
| `hobjdump.c`, `objdump_*.c` | the `hobjdump` program |
| `Makefile` | build rules |

## Author

Sash Munyankindi
