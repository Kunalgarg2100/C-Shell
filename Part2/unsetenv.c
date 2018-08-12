#include <stdlib.h>
#include <stdio.h>
#include "function.h"
int unset_env(char** args)
{
	if(args[1] == NULL){
		fprintf(stderr, "Usage: unset_env <varible>\n");
		return 1;
	}

	if(unsetenv(args[1])!=0)
		perror("Error: ");
	exit(EXIT_SUCCESS);

}

/*
Unsetenv
 The unsetenv() function deletes the variable name from the environment.  If name does not exist in the environment, then  the  func‐
       tion succeeds, and the environment is unchanged.
*/