# 0x0B. strace

A minimal reimplementation of `strace`, built on top of `ptrace(2)`. Each
program forks a child, has it request tracing, then `execve`s the given
command; the parent resumes the child one syscall at a time and prints
information about every syscall it intercepts.

## Environment

* OS: **Ubuntu 14.04 LTS**, compiled with **gcc 4.8.4**
* Flags: `-Wall -Werror -Wextra -pedantic`
* Allowed editors: `vi`, `vim`, `emacs`
* All files end with a new line
* x86_64 only (the syscall table and register layout used are specific
  to that architecture)

## Files

| File | Description |
| --- | --- |
| `syscalls.h` | Table of x86_64 syscalls: number, name, return type and parameter types |
| `strace_0.c` | Prints the **number** of each syscall made by the traced command |
| `strace_1.c` | Prints the **name** of each syscall made by the traced command |
| `strace_2.c` | Prints the **name and return value** (hex) of each syscall |
| `strace_3.c` | Prints the **name, parameters and return value** (hex) of each syscall |
| `Makefile` | Builds `strace_0`, `strace_1`, `strace_2`, `strace_3` |

## Compilation

```
make all
```

or individually: `make strace_0`, `make strace_1`, `make strace_2`, `make strace_3`.

## Usage

```
./strace_0 /bin/echo Holberton
./strace_1 /bin/echo Holberton
./strace_2 /bin/echo Holberton
./strace_3 /bin/echo Holberton
```

`command` must be given as a full path (no `$PATH` lookup is performed).

## Concepts

### What is `strace`, and what is its purpose?

`strace` is a diagnostic tool that traces the system calls a process makes
and the signals it receives, printing each one as it happens (name,
arguments, and return value). It's used to debug programs without source
access, understand what a binary is actually doing at the kernel boundary
(files it opens, syscalls it makes, why it fails), and to profile syscall
activity.

### What is `ptrace`, and what possibilities does it offer?

`ptrace(2)` is the Linux system call that lets one process (the tracer)
observe and control the execution of another (the tracee). Once attached
(via `PTRACE_TRACEME` in the child, or `PTRACE_ATTACH` from the tracer),
the tracee stops and reports its status to the tracer on every signal and,
if requested, on every syscall entry/exit. While stopped, the tracer can:

* read and write the tracee's registers (`PTRACE_GETREGS`/`PTRACE_SETREGS`)
* read and write the tracee's memory (`PTRACE_PEEKTEXT`/`PTRACE_POKETEXT`,
  or in bulk via `/proc/<pid>/mem`)
* single-step or run-until-next-syscall
* intercept and alter signals before they're delivered
* inject its own syscalls into the tracee

This is the same mechanism debuggers like `gdb` are built on.

### `PTRACE_SINGLESTEP` vs `PTRACE_SYSCALL`

Both resume a stopped tracee and stop it again at the next interesting
point, but they differ in what "next" means:

* `PTRACE_SINGLESTEP` resumes the tracee for exactly **one CPU
  instruction**, then delivers `SIGTRAP` again. It's useful for
  instruction-level debugging but far too slow/noisy for tracing syscalls,
  since most instructions have nothing to do with the kernel.
* `PTRACE_SYSCALL` resumes the tracee until it enters or exits a **system
  call** (i.e. executes a `syscall`/`int 0x80` instruction, or returns from
  one), ignoring every instruction in between. This is exactly the
  primitive `strace` needs: it stops twice per syscall — once on entry
  (registers hold the syscall number and arguments) and once on exit
  (registers hold the return value).

### Reading from / writing to the memory of a traced process

The tracer can inspect or modify the tracee's address space in two ways:

* `ptrace(PTRACE_PEEKTEXT, pid, addr, NULL)` reads one word at `addr` and
  returns it as the call's result; `ptrace(PTRACE_POKETEXT, pid, addr,
  data)` writes one word at `addr`. Reading/writing more than a word means
  looping over consecutive addresses.
* `/proc/<pid>/mem` can be opened and used with `pread`/`pwrite` at the
  target offset, which is more efficient for larger buffers than looping
  over `PEEKTEXT`/`POKETEXT` one word at a time.

This project only reads registers (`PTRACE_GETREGS`), since every value it
prints (syscall number, arguments, return value) lives in registers at the
syscall boundary — none of the four tasks need to dereference a pointer
argument to print out the memory it points to.
