#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include "function.h"

int kjob(char **args)
{
	if(args[1]==NULL || args[2] ==NULL)
	{
		fprintf(stderr, "Usage: kjob <jobid> <signal>\n");
		return 0;

	}

	int jbid = atoi(args[1]);
	if(jbid > max)
	{
		fprintf(stderr, "Error job number does not exist\n");
		return 0;
	}

	int sig=atoi(args[2]);
	kill(backgrund_process[jbid].pid,sig);
	if(sig == 9)
		backgrund_process[jbid].state=-1;
	else if(sig == 18)
		backgrund_process[jbid].state=0;
	return 1;
}