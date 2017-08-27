#include "cd.h"
#include<stdio.h>
#include<unistd.h>
#include<string.h>
int cd(char ** args)
{
	char * h = "/home";
	if(!args[1]){
		//printf("%c",*h);
		chdir(h);
		//prompt();
	}
	else if ((strcmp(args[1], "~")==0) || (strcmp(args[1], "~/")==0))
		chdir(h);

	else if(chdir(args[1])<0)
	{
			printf("bash: cd: %s: No such file or directory\n", args[1]);
			printf("bash: cd: %s: No such file or directory\n", args[1]);
	}

	return 1;
}




