#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

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
		 *cwd,
		 *home;

	user = getenv("USER");
	name = getenv("NAME");
	home = getenv("HOME");

	if (user == NULL || name == NULL || home == NULL)
	{
		strcpy(prompt_buffer, DEFAULT_PROMPT);	
		return prompt_buffer;
	}

	// get cwd
	size_t pwdbuf_size = 64;
	cwd = malloc(pwdbuf_size);
	while(1)
	{
		char *res = getcwd(cwd, pwdbuf_size);
		if( res == NULL)
		{
			if (errno == ERANGE)
			{
				// the buffer is too small
				pwdbuf_size += 64;
				cwd = realloc(cwd, pwdbuf_size);
				continue;
			}
			// other err
			perror("could not allocate buffer for pwd\n");
			exit(EXIT_FAILURE);
		} else // successfully allocated buffer
			break;
	} 


	// replace getenv("HOME") by ~ in pwd
	char *cwd_home_ptr = strstr(cwd, home);

	if (cwd_home_ptr-cwd == 0)
	{
		cwd = malloc(sizeof(cwd));
		strcpy(cwd, "~");
		strcat(cwd, cwd_home_ptr + strlen(home));

		free(cwd_home_ptr);
	}

	sprintf(prompt_buffer, "%s@%s:%s$ ", user, name, cwd);

	free(cwd);
	return prompt_buffer;
}
