#include <stdio.h> 
#include <dirent.h>
#include <fcntl.h>
#include <pwd.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <string.h>
#include <signal.h>
//start lsa.c
#include <sys/dir.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <grp.h>
#include <time.h>
#include <locale.h>
#include <langinfo.h>
#include <stdint.h>

#include "function.h"

int pinfo(char ** args)
{
	char * pinfo_pid;
	char * cmd1, * cmd2 , * cmd3;
	cmd1 = (char *)malloc(1111 * sizeof(char));
	cmd2 = (char *)malloc(1111 * sizeof(char));
	cmd3 = (char *)malloc(1111 * sizeof(char));
	//exec_path = (char *)malloc(1111 * sizeof(char));
	char exec_path[1111];
	//printf("size: %d\n",sizeof(exec_path));
	char ** args1, ** args2;
	if(args[1]!=NULL)
	{
		printf("pid -- %s\n", args[1]);
		sprintf(cmd1 ,"ps -o stat= -p %s",args[1]);
		args1 = split_line_fxn(cmd1);
		sprintf(cmd2 ,"ps -o vsz= -p %s",args[1]);
		args2 = split_line_fxn(cmd2);
		sprintf(cmd3 ,"/proc/%s/exe",args[1]);

	}
	else
	{
		int curr_pid = getpid();
		printf("pid -- %d\n", curr_pid);
		sprintf(cmd1 ,"ps -o stat= -p %d",curr_pid);
		args1 = split_line_fxn(cmd1);
		sprintf(cmd2 ,"ps -o vsz= -p %d",curr_pid);
		args2 = split_line_fxn(cmd2);
		sprintf(cmd3 ,"/proc/%d/exe",curr_pid);
	}

	printf("Process Status -- ");
	fflush(stdout);
	lsh_execute(args1);
	printf("Memory -- ");
	fflush(stdout);
	lsh_execute(args2);
	int p =readlink(cmd3 , exec_path , sizeof(exec_path));
	exec_path[p] = '\0';
	printf("Executable Path -- %s\n", exec_path);
	return 1;

}
