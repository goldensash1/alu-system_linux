# Multithreading

C project exploring POSIX threads (`pthread`): thread creation, safe
termination, mutual exclusion with mutexes, and using thread pools to speed up
CPU-bound work (image blurring and prime factorization).

## Requirements

- Ubuntu 14.04 LTS, `gcc 4.8.4`
- Compiled with `-Wall -Werror -Wextra -pedantic`
- Betty compliant (`betty-style.pl`, `betty-doc.pl`)
- No more than 5 functions per file
- All prototypes declared in `multithreading.h` (include-guarded)

## Concepts

**Thread** — the smallest unit a scheduler can run. Threads of the same
process share the same address space (heap, globals, file descriptors) but each
has its own stack and registers.

**Thread vs. process** — a process owns an isolated address space; creating one
is expensive and communication needs IPC. Threads live inside a process, share
its memory, are cheaper to create, and communicate through that shared memory —
which is why they need synchronization.

**Concurrency vs. parallelism** — concurrency is *dealing with* many tasks at
once (they make progress by interleaving, even on one core). Parallelism is
*doing* many tasks at the exact same instant on multiple cores. Concurrency is
about structure; parallelism is about execution.

**Creating a thread** — `pthread_create(&tid, attr, entry, arg)` starts a new
thread running `entry(arg)`.

**Exiting a thread** — return from the entry function, or call
`pthread_exit(retval)`. `pthread_join(tid, &retval)` waits for a thread and
collects its return value.

**Mutual exclusion** — a `pthread_mutex_t` lets only one thread enter a critical
section at a time (`pthread_mutex_lock` / `pthread_mutex_unlock`), preventing
concurrent access to shared data.

**Race condition** — a bug where the result depends on the unpredictable timing
of threads accessing shared data without synchronization (see task 4: without a
mutex, `tprintf` lines interleave).

**Deadlock** — two or more threads each hold a lock and wait forever for a lock
the other holds, so none can proceed.

## Files

| File | Description |
|------|-------------|
| `multithreading.h` | Header: data structures and all prototypes |
| `list.h`, `list.c` | Doubly linked list used by the prime-factor tasks |
| `0-thread_entry.c` | `thread_entry` — a new thread's entry point |
| `1-tprintf.c` | `tprintf` — logs a string prefixed with the thread ID |
| `10-blur_portion.c` | `blur_portion` — Gaussian-blur a portion of an image |
| `11-blur_image.c` | `blur_image` — blur a whole image with up to 16 threads |
| `20-tprintf.c` | `tprintf` v2 — mutex-guarded, set up via constructors |
| `21-prime_factors.c` | `prime_factors` — factorize a number into primes |
| `22-prime_factors.c` | Thread-pool task runner: `create_task`, `destroy_task`, `exec_tasks` |

## Compilation

```
# Task 0
gcc -Wall -Wextra -Werror -pedantic -g3 0-main.c 0-thread_entry.c -o 0-thread_entry -pthread

# Task 1
gcc -Wall -Wextra -Werror -pedantic -g3 1-main.c 1-tprintf.c -o 1-tprintf -pthread

# Task 2
gcc -Wall -Wextra -Werror -pedantic -g3 10-main.c 10-blur_portion.c -o 10-blur_portion

# Task 3
gcc -Wall -Wextra -Werror -pedantic -g3 11-main.c 11-blur_image.c 10-blur_portion.c -o 11-blur_image -pthread

# Task 4
gcc -Wall -Wextra -Werror -pedantic -g3 20-main.c 20-tprintf.c -o 20-tprintf -pthread

# Task 5
gcc -Wall -Wextra -Werror -pedantic -g3 21-main.c 21-prime_factors.c list.c -o 21-prime_factors

# Task 6
gcc -Wall -Wextra -Werror -pedantic -g3 22-main.c 22-prime_factors.c 21-prime_factors.c list.c 20-tprintf.c -o 22-prime_factors -pthread
```

## Author

Golden Sash — ALU System Programming
