#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include "function.h"

int echo(char **args)
{
	int i=1;
	if(args[i][0] == '\"' || args[i][0] == '\'')
	{
		int x = strlen(args[i]);

		
		if(args[i][x-1] == '\"' || args[i][x-1] == '\'')
			for(int j=1;j<x-1;j++)
				printf("%c",args[i][j]);
		else
			for(int j=1;j<x;j++)
			printf("%c",args[i][j]);
		printf(" ");

		i++;

		while(args[i]!=NULL)
		{
			int x = strlen(args[i]);
			if(args[i][x-1] == '\"' || args[i][x-1] == '\'')
			for(int j=0;j<x-1;j++)
				printf("%c",args[i][j]);
			else
			printf("%s ",args[i]);


			i++;

		}
		

	}


	while(args[i]!=NULL)
	{
	if(args[i][0]=='$')
	{
		const char* str=getenv(args[i]+1);
		if(str != NULL)
			printf("%s",str);
		else
			printf(" ");
	
	}



	else{

		printf("%s ",args[i]);
	}
		i++;
	}
	printf("\n");
	return 1;
}
