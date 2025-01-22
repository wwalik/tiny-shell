#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "p_readline.h"

#define BUFFER_SIZE 4
extern ssize_t p_readline(char** line_buf, size_t* lbuf_size)
{
	size_t lbuf_pos = 0;

	// malloc an initial buffer for input
	*lbuf_size = BUFFER_SIZE;
	*line_buf = (char*) malloc(*lbuf_size);
	if (*line_buf == NULL)
	{
		perror("could not allocate buffer\n");
		exit(EXIT_FAILURE);
	}

	char c;
	do
	{
		c = getchar();

		(*line_buf)[lbuf_pos] = c;
		lbuf_pos++;

		if (c == '\n') // handles enter
		{
			// Place string terminator
			(*line_buf)[lbuf_pos] = '\0';
			return lbuf_pos;
		}
		else if (c == EOF) // handles ctrl+d
		{
			printf("\n");
			exit(EXIT_SUCCESS);
		}

		// increase buffer size as needed
		if (lbuf_pos >= *lbuf_size-2) // -1 for string terminator
		{
			*lbuf_size += BUFFER_SIZE;
			char* new_lbuf = (char*) realloc(*line_buf, *lbuf_size);

			if (new_lbuf == NULL)
			{
				free(*line_buf);
				*line_buf = NULL;
				perror("could not reallocate buffer\n");
				exit(EXIT_FAILURE);
			}

			*line_buf = new_lbuf;
		}
	} while (c > 0);

	// return the error
	return c;
}
