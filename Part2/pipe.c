#include <stdio.h> 
#include <unistd.h> // fork()
#include <string.h>
#include <stdlib.h>
#include <sys/types.h> //open()
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "function.h"



int pipe_fxn(char **args)
{
	int i=0;
	int k=0;
	int noofcmd=0;
	char cmd [1111];
	int wpid;
	int status;
	int piid;
	int stdin_copy = dup(0);
	int stdout_copy = dup(1);

	piid = fork();
	if(piid == 0){
		while(args[i])
		{
			while(args[i] && strcmp(args[i],"|")!=0)
			{
				int n = strlen(args[i]);
				for(int j=0;j<n;j++)
					cmd[k++] = args[i][j];
				cmd[k++]=' ';
				i++;
			}
			if(args[i+1])
				cmd[k++]='|';

			i++;
		}
		cmd[k] = '\0';
		i=0;
		char ** pipargs = split_pipe_fxn(cmd);
		char ** args2;
		while(pipargs[i])
			i++;
		noofcmd = i;
		i=0;

		while(pipargs[i] && i < noofcmd-1)
		{
			args2 = split_line_fxn(pipargs[i]);
			int g=0;
			int input_redi = 0;
			int output_redi = 0;
			int append_redi = 0;

			while(args2[g]!=NULL)
			{
				if(strcmp(args2[g],"<") == 0)
					input_redi = g;
				if(strcmp(args2[g],">") == 0)
					output_redi =g;
				if(strcmp(args2[g],">>") == 0)
					append_redi=g;
				g++;
			}

			if(output_redi || append_redi)
			{
				int in=open(args2[input_redi+1],O_RDONLY);
				dup2(in,STDIN_FILENO);
				close(in);				
				return 1;
			}
			else
			{
				int pd[2];
				pipe(pd);
				int pid = fork();
				if(pid == 0)
				{
					dup2(pd[1],1);
					if(input_redi){
						if(execlp(args2[0],args2[0],args2[2],NULL) ==-1)
							kill(getpid(),SIGTERM);
					}
					else
						execvp(args2[0],args2);
				}
				dup2(pd[0], 0);
				close(pd[1]);
			}
			i++;

		}
		args2 = split_line_fxn(pipargs[i]);
		int input_redi = 0;
		int output_redi = 0;
		int append_redi = 0;
		int g=0;
		while(args2[i]!=NULL)
		{
			if(strcmp(args2[i],">") == 0)
				output_redi = i;
			if(strcmp(args2[i],">>") == 0)
				append_redi= i;
			i++;
		}

		if(output_redi || append_redi)
		{
			if(output_redi)
				g = output_redi;
			else
				g = append_redi;

			int f1;
			if(output_redi)
				f1 = open(args2[g+1],O_WRONLY | O_CREAT | O_TRUNC, 0644);
			else if(append_redi)
				f1 = open(args2[g+1],O_WRONLY | O_CREAT | O_APPEND, 0644);

			int wr=0;
			char * cmd1[6];
			while(strcmp(args2[wr],">") != 0 && strcmp(args2[wr],">>") != 0)
			{
				cmd1[wr] = args2[wr];
				wr++;
			}
			cmd1[wr] = NULL;

			close(1);
			dup2(f1,1);
			int pid = fork();
			pid_t wpid;
			int status;
			if(pid == 0)
				if(execvp(cmd1[0],cmd1) == -1)
					kill(getpid(),SIGTERM);

				else
				{
					do {
						wpid = waitpid(pid, &status, WUNTRACED);
					} while (!WIFEXITED(status) && !WIFSIGNALED(status));
				}
			close(f1);

		}
		else{
			//printf("%s\n",args2[0]);
			if(execvp(args2[0],args2) == -1)
				kill(getpid(),SIGTERM);
		}
		exit(0);
	}
	else
	{

		do {
			wpid = waitpid(piid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	dup2(stdin_copy, 0);
	dup2(stdout_copy, 1);
	close(stdin_copy);
	close(stdout_copy);
	return 1;
}

