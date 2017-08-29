#include <stdio.h> 
#include "function.h"

int echo(char **args)
{
	int i=1;
	while(args[i]!=NULL)
	{
	if(args[i][0]=='$')
	{
		const char* str=getenv(args[i]+1);
		if(str != NULL)
			printf("%s",str);
		else
			printf(" ",str);
	
	}
	else{

		printf("%s ",args[i]);
	}
		i++;
	}
	printf("\n");
	return 1;
}
