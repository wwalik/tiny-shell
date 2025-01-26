#pragma once

// returns a malloc'd address of the prompt string
#define PROMPT_BUFFER_SIZE 256
#define DEFAULT_PROMPT "$ "
extern const char*
get_prompt();
