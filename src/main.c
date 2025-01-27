#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "handle_input.h"
#include "prompt.h"

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
