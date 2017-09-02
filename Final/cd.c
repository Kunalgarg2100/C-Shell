#include <stdio.h> 
#include <unistd.h> 
#include <string.h>
#include "function.h"
/*
chdir() works as follows:
1. A new process is spawned to run your program
2. That program calls chdir(), to change its current directory
3. The program end, causing the process to die
4. The shell is still where it was
chdir() changes the current working directory of the calling process to the directory specified in path.
chdir  On success, zero is returned.  On error, -1 is returned, and errno is set appropriately.
perror prints a description for a system error code or for a storage engine (table handler) error code.
The chdir command works, but only inside your program. Once you exit it, you're back to the parent shell current directory.

*/

int cd(char ** args)
{	
	char cwdc[1111];
	getcwd(cwdc,sizeof(cwdc));
	int x=0;
	char  h[1000] ;
	strcpy(h,home);
	if(!args[1])
	{	
		x = chdir(h);
		return 1;
		//prompt();
	}
	else if ((strcmp(args[1],"~")==0) || (strcmp(args[1], "~/")==0))
	{
		x =  chdir(h);
		return 1;
	}
	else if((strcmp(args[1],"-"))==0)
	{
		x=chdir(previous);
		printf("%s\n",previous);
		strcpy(previous,cwdc);
		return 1;
	}
	strcpy(previous,cwdc);
	x = chdir(args[1]);
	if(x<0)
		perror("Error ");
	background_fxn();
	return 1;
}
