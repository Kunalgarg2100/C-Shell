#include <stdio.h> 
#include <unistd.h> 
#include <string.h>
#include "function.h"

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
		printf("bash: cd: %s: No such file or directory\n", args[1]);
	background_fxn();
	return 1;
}
