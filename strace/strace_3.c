#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include "syscalls.h"

extern char **environ;

/**
 * get_syscall - looks up a syscall description by its number
 * @num: the syscall number (value of orig_rax)
 *
 * Return: pointer to the matching syscall_t, or NULL if not found
 */
syscall_t const *get_syscall(unsigned long num)
{
	size_t i, count;

	count = sizeof(syscalls_64_g) / sizeof(syscalls_64_g[0]);
	for (i = 0; i < count; i++)
		if (syscalls_64_g[i].nr == num)
			return (&syscalls_64_g[i]);
	return (NULL);
}

/**
 * print_syscall - prints the name and parameters of a syscall
 * @sc: the syscall description, or NULL if unknown
 * @regs: the traced process' registers at syscall-enter
 *
 * The closing parenthesis is intentionally not printed here: it is printed
 * at syscall-exit next to the return value, so that any output produced by
 * the traced child in-between interleaves at the right place.
 *
 * Return: nothing
 */
void print_syscall(syscall_t const *sc, struct user_regs_struct const *regs)
{
	unsigned long args[6];
	size_t i;

	args[0] = regs->rdi;
	args[1] = regs->rsi;
	args[2] = regs->rdx;
	args[3] = regs->r10;
	args[4] = regs->r8;
	args[5] = regs->r9;

	printf("%s(", sc ? sc->name : "?");
	if (sc && !(sc->nb_params == 1 && sc->params[0] == VOID))
	{
		for (i = 0; i < sc->nb_params; i++)
		{
			if (i > 0)
				printf(", ");
			printf("%#lx", args[i]);
		}
	}
}

/**
 * start_child - forks and starts tracing the command to execute
 * @argv: argument vector, argv[1] is the full path of the command to run
 *
 * Return: the pid of the newly created child
 */
pid_t start_child(char **argv)
{
	pid_t child;

	child = fork();
	if (child == -1)
	{
		perror("fork");
		_exit(1);
	}
	if (child == 0)
	{
		ptrace(PTRACE_TRACEME, 0, NULL, NULL);
		execve(argv[1], &argv[1], environ);
		perror(argv[1]);
		_exit(1);
	}
	return (child);
}

/**
 * run_trace - resumes a traced child until it exits, printing the
 * name, parameters and return value of each syscall it makes
 * @child: pid of the traced child
 *
 * Return: nothing
 */
void run_trace(pid_t child)
{
	int status, is_entry, pending;
	struct user_regs_struct regs;

	is_entry = 1;
	pending = 0;
	while (1)
	{
		if (ptrace(PTRACE_SYSCALL, child, NULL, NULL) == -1)
			break;
		waitpid(child, &status, 0);
		if (WIFEXITED(status) || WIFSIGNALED(status))
		{
			if (pending)
			{
				printf(") = ?\n");
				fflush(stdout);
			}
			break;
		}
		ptrace(PTRACE_GETREGS, child, NULL, &regs);
		if (is_entry)
		{
			print_syscall(get_syscall(regs.orig_rax), &regs);
			fflush(stdout);
			pending = 1;
		}
		else
		{
			printf(") = %#lx\n", (unsigned long)regs.rax);
			fflush(stdout);
			pending = 0;
		}
		is_entry = !is_entry;
	}
}

/**
 * main - executes and traces a command, printing the name,
 * parameters and return value (in hexadecimal) of each syscall it makes
 * @argc: number of arguments
 * @argv: argument vector, argv[1] is the full path of the command to run
 *
 * Return: 0 on success, 1 on error
 */
int main(int argc, char **argv)
{
	pid_t child;
	int status;
	struct user_regs_struct regs;

	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s command [args...]\n", argv[0]);
		return (1);
	}

	child = start_child(argv);
	waitpid(child, &status, 0);
	if (WIFEXITED(status) || WIFSIGNALED(status))
		return (0);
	ptrace(PTRACE_GETREGS, child, NULL, &regs);
	print_syscall(get_syscall(regs.orig_rax), &regs);
	printf(") = %#lx\n", (unsigned long)regs.rax);
	fflush(stdout);

	run_trace(child);
	return (0);
}
