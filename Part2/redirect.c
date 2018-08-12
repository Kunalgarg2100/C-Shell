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
			input_redi = i;
		if(strcmp(args[i],">") == 0)
			output_redi = i;
		if(strcmp(args[i],">>") == 0)
			append_redi= i;
		i++;
	}

	int stdin_copy = dup(0);
	int stdout_copy = dup(1);
	close(0);
	close(1);

	if(output_redi || append_redi)
	{ //There is output redirection
		int f1;
		char * cmd1[100];
		if(output_redi)
			f1 = open(args[output_redi+1],O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if(append_redi)
			f1 = open(args[append_redi+1],O_WRONLY | O_CREAT | O_APPEND, 0644);
		if(f1 == -1)
			perror("open error: ");
		fflush(stdout);
		dup2(f1,1);	
		if(input_redi)
		{
			int wr=0;
				while(strcmp(args[wr],"<") != 0)
				{	
					cmd1[wr] = args[wr];
					wr++;
				}
				cmd1[wr] = args[input_redi+1];
				cmd1[wr+1] = NULL;

		}

		else
		{
			
				int wr=0;
				while(strcmp(args[wr],">") != 0 && strcmp(args[wr],">>") != 0)
				{	
					cmd1[wr] = args[wr];
					wr++;
				}
				cmd1[wr] = NULL;
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
		char * cmd1[100];

		int wr=0;
				while(strcmp(args[wr],"<") != 0)
				{	
					cmd1[wr] = args[wr];
					wr++;
				}
				cmd1[wr] = args[input_redi+1];
				cmd1[wr+1]=NULL;

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
	return 1;
}