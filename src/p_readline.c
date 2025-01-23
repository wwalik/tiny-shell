#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "p_readline.h"

#define BUFFER_DEFAULT_SIZE 16
#define BUFFER_INCREMENT 8
extern ssize_t p_readline(char** lbuf, size_t* lbuf_size)
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
		c = getchar();

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
