#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "prompt.h"

extern const char*
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
		 *pwd,
		 *home;

	user = getenv("USER");
	name = getenv("NAME");
	pwd = getenv("PWD");
	home = getenv("HOME");

	if (user == NULL || name == NULL || pwd == NULL || home == NULL)
	{
		perror("could not get env variables for prompt: defaulting to '$ ' prompt\n");
		strcpy(prompt_buffer, DEFAULT_PROMPT);	
	}

	// replace getenv("HOME") by ~ in pwd
	// i will need to implement a custom function
	// this assumes the home directory is similar to /home/username/
	char *pwdbuf_home_ptr = strstr(pwd, home);

	pwd = malloc(sizeof(pwd)+1);
	strcpy(pwd, "~");
	strcat(pwd, pwdbuf_home_ptr + strlen(home));


	sprintf(prompt_buffer, "%s@%s:%s$ ", user, name, pwd);

	free(pwd);
	return prompt_buffer;
}
