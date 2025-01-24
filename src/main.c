#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "p_readline.h"
#include "execute.h"

// returns a malloc'd address of the prompt string
#define PROMPT_BUFFER_SIZE 256
#define DEFAULT_PROMPT "$ "
char*
get_prompt()
{
	static char *prompt_buffer = NULL;
	static size_t pbuf_size = PROMPT_BUFFER_SIZE;

	// if prompt_buffer was never initialized
	// this is obviously inefficient i think i shouldve made the buffer a global struct or smth like that?
	if (prompt_buffer == NULL)
	{
		prompt_buffer = malloc(pbuf_size);
		if (prompt_buffer == NULL)
			exit(EXIT_FAILURE);
	}

	// get env variables
	char *user,
		 *name,
		 *pwd;

	user = getenv("USER");
	name = getenv("NAME");
	pwd = getenv("PWD");

	// replace getenv("HOME") by ~ in pwd
	// i will need to implement a custom function
	
	if (user == NULL || name == NULL || pwd == NULL)
	{
		perror("could not get env variables for prompt: defaulting to '$ ' prompt\n");
		strcpy(prompt_buffer, DEFAULT_PROMPT);	
	}

	// reset prompt string
	memset(prompt_buffer, 0, strlen(prompt_buffer));
	
	sprintf(prompt_buffer, "%s@%s:%s$ ", user, name, pwd);


	return prompt_buffer;
}

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
		char *prompt_str;
		prompt_str = get_prompt();

		printf("%s", prompt_str);

	 	free(prompt_str);
	 	prompt_str = NULL;



		// READ INPUT
		len = p_readline(&lbuf, &lbuf_size, stdin);
		if (len == EOF)
			break;
		if (len <= 1) // EMPTY LINE
			continue;

		// PROCESS INPUT
		handle_input(&lbuf, &status);
		printf("res: %d\n", status);
	}

	free(lbuf);
	lbuf = NULL;

	exit(EXIT_SUCCESS);
}
