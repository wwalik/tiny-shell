#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#include "p_readline.h"

#define PROMPT "$ "

void
parse_input(char** lbuf, size_t* lbuf_size)
{

}

int
main()
{
	size_t lbuf_size;
	char* lbuf;
	ssize_t len; 

	while(1)
	{
		// DISPLAY PROMPT
		printf("%s", PROMPT);

		// READ INPUT
		len = p_readline(&lbuf, &lbuf_size);
		if (len == EOF)
			break;

		// PROCESS INPUT
		parse_input(&lbuf, &lbuf_size);
	}


	printf("allocated a buffer of size %ld and read %ld characters: %s", lbuf_size, len, lbuf);

	free(lbuf);
	lbuf = NULL;

	exit(EXIT_SUCCESS);
}
