#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "p_readline.h"
#include "execute.h"

#define PROMPT "$ "

// returns 0 if execution was successful, -1 otherwise
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

	// handle input
	int res = execute(tokens, status);
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
		if (len == EOF)
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
