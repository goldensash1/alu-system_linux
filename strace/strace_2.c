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
 * print_name - prints the name of a syscall, or its raw number if unknown
 * @num: the syscall number
 *
 * Return: nothing
 */
void print_name(unsigned long num)
{
	syscall_t const *sc;

	sc = get_syscall(num);
	printf("%s", sc ? sc->name : "?");
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
 * name and return value of each syscall it makes
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
				printf(" = ?\n");
				fflush(stdout);
			}
			break;
		}
		ptrace(PTRACE_GETREGS, child, NULL, &regs);
		if (is_entry)
		{
			print_name(regs.orig_rax);
			fflush(stdout);
			pending = 1;
		}
		else
		{
			printf(" = %#lx\n", (unsigned long)regs.rax);
			fflush(stdout);
			pending = 0;
		}
		is_entry = !is_entry;
	}
}

/**
 * main - executes and traces a command, printing the name and
 * return value (in hexadecimal) of each syscall it makes
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
	print_name(regs.orig_rax);
	printf(" = %#lx\n", (unsigned long)regs.rax);
	fflush(stdout);

	run_trace(child);
	return (0);
}
