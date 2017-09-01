#include <stdio.h> 
#include <stdlib.h>
#include <sys/types.h> 
#include <unistd.h> 
#include "function.h"

int pinfo(char ** args)
{
	char * pinfo_pid;
	char * cmd1, * cmd2 , * cmd3;
	cmd1 = (char *)malloc(1111 * sizeof(char));
	cmd2 = (char *)malloc(1111 * sizeof(char));
	cmd3 = (char *)malloc(1111 * sizeof(char));
	char exec_path[1111];
	char ** args1, ** args2;
	if(args[1]!=NULL)
	{
		printf("pid -- %s\n", args[1]);
		sprintf(cmd1 ,"ps -o stat= --pid %s",args[1]);
		args1 = split_line_fxn(cmd1);
		sprintf(cmd2 ,"ps -o vsz= --pid %s",args[1]);
		args2 = split_line_fxn(cmd2);
		sprintf(cmd3 ,"/proc/%s/exe",args[1]);

	}
	/*
	  --pid pidlist : Selects the processes by process ID
	  -o format
              User-defined format. format is a single argument in the form of a blank-separated or comma-separated list, which offers a
              way to specify individual output columns.  
		
      vsz : virtual memory size of the process in KiB (1024-byte units).  Device mappings are currently excluded; this is
                             subject to change.  (alias vsize).

*/
	else
	{
		int curr_pid = getpid();
		printf("pid -- %d\n", curr_pid);
		sprintf(cmd1 ,"ps -o stat= --pid %d",curr_pid);
		args1 = split_line_fxn(cmd1);
		sprintf(cmd2 ,"ps -o vsz= --pid %d",curr_pid);
		args2 = split_line_fxn(cmd2);
		sprintf(cmd3 ,"/proc/%d/exe",curr_pid);
	}

	printf("Process Status -- ");
	fflush(stdout);
	execute_func(args1);
	printf("Memory -- ");
	fflush(stdout);
	execute_func(args2);
	int p =readlink(cmd3 , exec_path , sizeof(exec_path));
	exec_path[p] = '\0';
	printf("Executable Path -- %s\n", exec_path);
	return 1;

}
