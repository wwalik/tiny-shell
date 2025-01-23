#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>

#include "execute.h"

extern int
execute(char **argv, int *status)
{
	pid_t pid;

	pid = fork();
	if (pid == -1)
	{
		perror("could not fork\n");
		exit(EXIT_FAILURE);
	} else if (pid == 0)
	{
		// CHILD
		int exec_e = execvp(argv[0], argv);
		
		// did not execute
		if (errno == ENOENT)
		{
			printf("could not find file or directory\n");
		} else
		{
			fprintf(stderr, "could not execute program, errno:%d\n", errno);
		}

		exit(EXIT_FAILURE);
	} else
	{
		struct sigaction new_sa;
		struct sigaction old_sa;
		int sa_e;

		// temporarily ignore CTRL+C while child is running
		new_sa.sa_handler = SIG_IGN;
		sigemptyset(&new_sa.sa_mask);
		new_sa.sa_flags = 0;

		sa_e = sigaction(SIGINT, &new_sa, &old_sa);
		if (sa_e == -1)
		{
			perror("set sigaction error\n");
			exit(EXIT_FAILURE);
		}

		// PARENT
		int wstatus;
		waitpid(pid, &wstatus, 0); // stop until the program has executed

		*status = WEXITSTATUS(wstatus);

		// Reset sig handler
		sa_e = sigaction(SIGINT, &old_sa, NULL);
		if (sa_e == -1)
		{
			perror("reset sigaction error\n");
			exit(EXIT_FAILURE);
		}
	}

	// execution was successful
	return 0;
}

