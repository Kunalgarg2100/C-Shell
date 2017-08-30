#include <stdio.h> 
#include <unistd.h> 
#include "function.h"

int pwd(char ** args)
{
	int i=1;
	int flag=0;
	while(args[i] !=NULL)
	{
		if(args[i][0] == '&')
			{
				//lsh_launch(args,1);
				flag=1;
			}
		i++;

	}
	if(flag == 1)
		back_process(getpid(),"pwd");
	char cwd[1111];
	getcwd(cwd,sizeof(cwd));
	printf("%s\n",cwd);
	background_fxn();
    return 1;
}
