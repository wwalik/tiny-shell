#pragma once
#include <sys/types.h>
#include <stdio.h>

extern ssize_t p_readline(char** line_buffer, size_t* lbuf_size, FILE *stream);
