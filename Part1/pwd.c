#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h>
#include "function.h"

int pwd(char ** args)
{
	char cwd[1111];
	getcwd(cwd,sizeof(cwd));
	/*  
		The getcwd() function copies an absolute pathname of the current working directory to the array 
		pointed  to  by  cwd. On  success,  it returns  a pointer to a string containing the pathname of 
		the current working directory. On failure, these functions return NULL.
	*/
	printf("%s\n",cwd);
    return 1;
}
