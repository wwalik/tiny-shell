#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "p_readline.h"
#include "execute.h"
#include "prompt.h"

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
	if (strcmp(tokens[0], "cd") == 0)
	{
		if (tokens_len == 2)
		{
			int res = chdir(tokens[1]);
			printf("cd res: %d\n", res);
		}
		else
			printf("cd requires at least 1 argument\n");
	} else
	{
		int res = execute(tokens, status);
	}
}


int
main()
{
	char* line_buffer;
	size_t lbuf_size;
	ssize_t len; 

	// status of the last executed command
	int status = 0;
	while(1)
	{
		fflush(stdin);
		fflush(stdout);

		
		// DISPLAY PROMPT
		const char *prompt_str;

		prompt_str = get_prompt();
		printf("%s", prompt_str);


		// READ INPUT
		len = p_readline(&line_buffer, &lbuf_size, stdin);
		if (len == EOF) // CTRL+D
			break;
		if (len <= 1) // EMPTY LINE
			continue;

		// PROCESS INPUT
		handle_input(&line_buffer, &status);
		printf("res: %d\n", status);
	}

	free(line_buffer);
	line_buffer = NULL;

	exit(EXIT_SUCCESS);
}
