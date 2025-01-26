#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "prompt.h"

extern char*
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
