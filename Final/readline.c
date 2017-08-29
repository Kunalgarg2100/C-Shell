#include <stdio.h> 
//start lsa.c

#include "function.h"

char *read_line(void)
{
	char *line = NULL;
	ssize_t bufsize = 0; // have getline allocate a buffer for us
	getline(&line, &bufsize, stdin);
	return line;
}
