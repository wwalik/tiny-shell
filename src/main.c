#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "p_readline.h"
#include "execute.h"
#include "prompt.h"

static void
translate_input(char **tokens, size_t tokens_len)
{
	// look for ~ and $
	for (size_t i = 0; i < tokens_len; i++)
	{
		char *tilde_ptr = strchr(tokens[i], '~');
		char *dollar_ptr = strchr(tokens[i], '$');

		if (tilde_ptr != NULL)
		{
			char *home = getenv("HOME");
			if (home == NULL)
			{
				printf("could not find home directory\n");
				return;
			}
			
			char *new_buf = malloc(strlen(tokens[i])+strlen(home));
			new_buf = strncpy(new_buf, tokens[i], tilde_ptr-tokens[i]);
			strcat(new_buf, home);
			strcat(new_buf, tilde_ptr+1);

			tokens[i] = new_buf;
		}

		if (dollar_ptr != NULL)
		{
			printf("found a $\n");
			printf("env variables substitution isnt yet implemented\n");
		}
	}
}


#define TOKEN_MAX 512
void
handle_input(char **lbuf, int *status)
{
	// PARSING
	char *tokens[TOKEN_MAX];
	size_t tokens_len = 0;
	for(char *token = strtok(*lbuf, " \n\t");
			token != NULL || tokens_len >= TOKEN_MAX-1; // -1 for NULL byte of args
			token = strtok(NULL, " \n\t")) 
	{
		tokens[tokens_len++] = token;
	}

	tokens[tokens_len] = NULL; // tells execvp when to stop looking for arguments

	translate_input(tokens, tokens_len);
	// handle input
	if (strcmp(tokens[0], "cd") == 0)
	{
		// cd
		switch (tokens_len)
		{
			int res;
			case 2:
				res = chdir(tokens[1]);
				if (res == 0)
					break;

				if (errno == ENOENT)
					printf("could not find directory\n");
				else
					printf("other errno: %d\n", errno);
				break;

			case 1: ;
				char *home = getenv("HOME");
				if (home == NULL)
				{
					printf("no home directory\n");
					break;
				}

				res = chdir(home);
				if (res != 0)
					printf("could not find directory\n");
				break;

			default:
				printf("cd requires at least 1 argument\n");
				break;

		}
	} else
	{
		// any other command
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
