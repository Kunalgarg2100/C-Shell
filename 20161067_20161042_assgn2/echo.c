#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include "function.h"

int echo(char **args)
{
	if(args[1] == NULL)
	{
		printf("\n");
		return 1;
	}
	int i=1;
	/* This part takes care if the argument starts with " or '*/
	if(args[i][0] == '\"' || args[i][0] == '\'')  // If argument starts with " or '
	{
		int x = strlen(args[i]); 
		if(args[i][x-1] == '\"' || args[i][x-1] == '\'')
			for(int j=1;j<x-1;j++)
				printf("%c",args[i][j]); // If the argument ends with " or '
		else
			for(int j=1;j<x;j++)
				printf("%c",args[i][j]);
		printf(" "); // space 

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
	
	else
	{
		while(args[i]!=NULL)
		{
			if(args[i][0]=='$') // If argument starts with '$' then we print its value from the environment
			{
				const char* str=getenv(args[i]+1);
				if(str != NULL)
					printf("%s",str);
				else
					printf(" ");
			}
			else // If argument doesn't start with '$' then it is printed as it is
				printf("%s ",args[i]);
			i++;
		}
	}
	printf("\n");
	return 1;
}
