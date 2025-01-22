#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 4
ssize_t p_getline(char** line_buf, size_t* lbuf_size)
{
	size_t lbuf_pos = 0;

	// malloc an initial buffer for input
	*line_buf = malloc(BUFFER_SIZE);
	*lbuf_size = BUFFER_SIZE;
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
			// *line_buf[lbuf_pos] = '\0';
			return lbuf_pos;
		}
		else if (c == EOF) // handles ctrl+d
		{
			printf("\n");
			exit(EXIT_SUCCESS);
		}

		// increase buffer size as needed
		if (lbuf_pos+1 >= *lbuf_size) // +1 for string terminator
		{
			*line_buf = realloc(*line_buf, *lbuf_size + BUFFER_SIZE);
			*lbuf_size += BUFFER_SIZE;
			if (*line_buf == NULL)
			{
				perror("could not reallocate buffer\n");
				exit(EXIT_FAILURE);
			}
		}
	} while (c > 0);

	// return the error
	return c;
}

int
main()
{

	char* lbuf;
	size_t lbuf_size;
	ssize_t len;

	len = p_getline(&lbuf, &lbuf_size);

	printf("allocated a buffer of size %ld and read %ld characters: %s", lbuf_size, len, lbuf);

	exit(EXIT_SUCCESS);
}
