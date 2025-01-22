#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#include "p_readline.h"

int
main()
{
	char* lbuf;
	size_t lbuf_size = 0;
	ssize_t len;

	len = p_readline(&lbuf, &lbuf_size);

	printf("allocated a buffer of size %ld and read %ld characters: %s", lbuf_size, len, lbuf);

	free(lbuf);
	lbuf = NULL;

	exit(EXIT_SUCCESS);
}
