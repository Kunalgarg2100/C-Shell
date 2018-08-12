#include <stdio.h> 
#include "function.h"
/*
Syntax:- ssize_t getline(char **lineptr, size_t *n, FILE *stream);
getline() reads  an  entire  line  from  stream,  storing  the address  of the buffer 
containing the text into *lineptr.  The buffer is null-terminated and includes the newline
character, if one was found. If  *lineptr  is  set to NULL and *n is set 0 before the call,
then getline() will allocate a buffer for  storing  the  line.
getline also resizes the buffer with realloc(3), updating *lineptr and *n as necessary.
On  success,  getline() return the number of characters read, including the delimiter
returns -1 on failure to read a line
*/

char *read_line(void)
{
	char *line = NULL;
	ssize_t bufsize = 0; // have getline allocate a buffer for us
	getline(&line, &bufsize, stdin);
	return line;
}
