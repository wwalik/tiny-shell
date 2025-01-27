#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "handle_input.h"
#include "execute.h"

#define BUFFER_DEFAULT_SIZE 16
#define BUFFER_INCREMENT 8
extern ssize_t
p_readline(char** lbuf, size_t* lbuf_size, FILE *stream)
{
	size_t lbuf_pos = 0;

	// malloc an initial buffer for input
	if (*lbuf == NULL || *lbuf_size == 0) // lbuf_size cannot be negative
	{
		*lbuf_size = BUFFER_DEFAULT_SIZE;
		*lbuf = (char*) malloc(*lbuf_size);
		if (*lbuf == NULL)
		{
			perror("could not allocate buffer\n");
			exit(EXIT_FAILURE); // TODO: handle the error
		}
	}

	char c;
	do
	{
		c = fgetc(stream);

		(*lbuf)[lbuf_pos] = c;
		lbuf_pos++;

		if (c == '\n') // handles enter
		{
			// Place string terminator
			(*lbuf)[lbuf_pos] = '\0';
			return lbuf_pos;
		}
		else if (c == EOF) // handles ctrl+d
		{
			printf("\n");
			return EOF;
		}

		// increase buffer size as needed
		if (lbuf_pos >= *lbuf_size-2) // -1 for string terminator
		{
			*lbuf_size += BUFFER_INCREMENT;
			char* new_lbuf = (char*) realloc(*lbuf, *lbuf_size);

			if (new_lbuf == NULL)
			{
				free(*lbuf);
				*lbuf = NULL;
				perror("could not reallocate buffer\n");
				exit(EXIT_FAILURE); // TODO: Handle the error
			}

			*lbuf = new_lbuf;
		}
	} while (c > 0); // get_char() doesn't return any error information other than EOF (-1) so this is pointless ferror() should be called instead

	// return the "error"
	return c;
}

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
extern void
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
