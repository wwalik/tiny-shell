#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <signal.h>

#include "p_readline.h"

#define PROMPT "$ "

#define TOKEN_MAX 512
void
handle_input(char **lbuf, int *status)
{
	// PARSING
	char *token;
	token = strtok(*lbuf, " \n\t");

	char *tokens[TOKEN_MAX];
	size_t tokens_len = 0;
	for(; token != NULL || tokens_len >= TOKEN_MAX-1; token = strtok(NULL, " \n\t")) // -1 for NULL byte of args
		tokens[tokens_len++] = token;

	tokens[tokens_len] = NULL; // tells execvp when to stop looking for arguments

	// check if user has inputed something
	if (tokens_len == 0)
		return;

	// HANDLE COMMAND
	pid_t pid;

	pid = fork();
	if (pid == -1)
	{
		perror("could not fork\n");
		exit(EXIT_FAILURE);
	} else if (pid == 0)
	{
		// CHILD
		execvp(tokens[0], tokens);
		
		// did not execute
		if (errno == ENOENT)
		{
			printf("could not find file or directory\n");
		} else
		{
			fprintf(stderr, "could not execute program, errno:%d\n", errno);
		}
		
		// Kill child
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
	
	return;
}

int
main()
{
	size_t lbuf_size;
	char* lbuf;
	ssize_t len; 

	int status = 0;
	while(1)
	{
		fflush(stdin);
		fflush(stdout);
		// DISPLAY PROMPT
		printf("%s", PROMPT);

		// READ INPUT
		len = p_readline(&lbuf, &lbuf_size);
		if (len == EOF) // CTRL+D
			break;
		if (len <= 1) // EMPTY LINE
			continue;

		// PROCESS INPUT
		handle_input(&lbuf, &status);
		printf("res: %d\n", status);
	}


	//	printf("allocated a buffer of size %ld and read %ld characters: %s", lbuf_size, len, lbuf);

	free(lbuf);
	lbuf = NULL;

	exit(EXIT_SUCCESS);
}
