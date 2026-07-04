#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/user.h>

extern char **environ;

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
 * number of each syscall it makes on syscall-enter
 * @child: pid of the traced child
 *
 * Return: nothing
 */
void run_trace(pid_t child)
{
	int status, is_entry;
	struct user_regs_struct regs;

	is_entry = 1;
	while (1)
	{
		if (ptrace(PTRACE_SYSCALL, child, NULL, NULL) == -1)
			break;
		waitpid(child, &status, 0);
		if (WIFEXITED(status) || WIFSIGNALED(status))
			break;
		ptrace(PTRACE_GETREGS, child, NULL, &regs);
		if (is_entry)
		{
			printf("%lu\n", (unsigned long)regs.orig_rax);
			fflush(stdout);
		}
		is_entry = !is_entry;
	}
}

/**
 * main - executes and traces a command, printing the number of
 * each syscall it makes
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
	printf("%lu\n", (unsigned long)regs.orig_rax);
	fflush(stdout);

	run_trace(child);
	return (0);
}
