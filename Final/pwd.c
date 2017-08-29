#include <stdio.h> 
#include <unistd.h> 
#include "function.h"

int pwd(char ** args)
{
	char cwd[1111];
	getcwd(cwd,sizeof(cwd));
	printf("\n%s\n",cwd);
    return 1;
}
