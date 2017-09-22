#include <unistd.h> // fork()
#include <string.h> //strcmp
#include <sys/types.h> //open()
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <fcntl.h>
#include "function.h"

int redirect_fxn(char **args)
{
	int i=0;
	int input_redi = 0;
	int output_redi = 0;
	int append_redi = 0;

	while(args[i]!=NULL)
	{
		if(strcmp(args[i],"<") == 0)
			input_redi = 1;
		if(strcmp(args[i],">") == 0)
			output_redi =1;
		if(strcmp(args[i],">>") == 0)
			append_redi=1;
		i++;
	}

	int stdin_copy = dup(0);
	int stdout_copy = dup(1);
	close(0);
	close(1);

	if(output_redi || append_redi)
	{ //There is output redirection
		int f1;
		char * cmd1[6];
		if(output_redi)
			f1 = open(args[4],O_WRONLY | O_CREAT, 0644);
		else if(append_redi)
			f1 = open(args[4],O_WRONLY | O_CREAT | O_APPEND, 0644);
		dup2(f1,1);	
		if(input_redi)
		{
			if(strcmp(args[0],"sort") == 0){
				cmd1[0] = "sort";
				cmd1[1]= args[2];
				cmd1[2] =NULL;
			}
		}

		else
		{
			if(strcmp(args[0],"diff") == 0)
			{	cmd1[0] = "diff";
				cmd1[1]= args[1];
				cmd1[2] = args[2];
				cmd1[3] = NULL;
			}
		}

		int pid = fork();
		pid_t wpid;
		int status;
		if(pid == 0)
			execvp(cmd1[0],cmd1);
		else
		{
			do {
				wpid = waitpid(pid, &status, WUNTRACED);
			} while (!WIFEXITED(status) && !WIFSIGNALED(status));
		}
		close(f1);
		dup2(stdin_copy, 0);
		dup2(stdout_copy, 1);
		close(stdin_copy);
		close(stdout_copy);
	}


	else if(input_redi)
	{
		dup2(stdin_copy, 0);
		dup2(stdout_copy, 1);
		close(stdin_copy);
		close(stdout_copy);

		if(strcmp(args[0],"sort") == 0)
		{
			char * cmd1[] = {"sort", args[2] ,NULL};
			int pid = fork();
			int status;
			pid_t wpid;
			if(pid == 0)
				execvp(cmd1[0],cmd1);
			else
			{
				do {
					wpid = waitpid(pid, &status, WUNTRACED);
				} while (!WIFEXITED(status) && !WIFSIGNALED(status));
			}
		}
	}
	return 1;
}