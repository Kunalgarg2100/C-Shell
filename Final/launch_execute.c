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
#include <sys/wait.h>

#include <sys/stat.h>
#include <grp.h>
#include <time.h>
#include <locale.h>
#include <langinfo.h>
#include <stdint.h>

#include "function.h"
int max;

char *builtin_str[] = {
	"cd","pwd","echo","ls","exit_shell","pinfo","nightswatch"
};
int (*builtin_func[]) (char **) = {
	&cd,&pwd,&echo,&ls,&exit_shell,&pinfo,&nightswatch
};

int num_builtins() {
	return sizeof(builtin_str) / sizeof(char *);
}
void background_fxn()
{
	pid_t wpid;
	int status, i;
	char pidnumber[1000];

	while((wpid = waitpid(-1, &status, WNOHANG)) > 0)
	{
		//printf("wpid: -%d      luluuuu   \n",wpid );
		if(WIFEXITED(status) == 0)
		{
			for(i = 1; i <=max; i++)
			{
				if(backgrund_process[i].pid == wpid)
				{
					strcpy(pidnumber, backgrund_process[i].command);
					backgrund_process[i].state = -1;
					printf("[%d]	Done\t\t\t%s\n",backgrund_process[i].jobid, pidnumber);
				}
			}
			prompt();
		}
		if(WIFSIGNALED(status) == 0)
		{
			for(i = 1; i <=max; i++)
			{
				if(backgrund_process[i].pid == wpid)
				{
					strcpy(pidnumber, backgrund_process[i].command);
					backgrund_process[i].state = -1;
					printf("[%d]	Done\t\t\t%s\n",backgrund_process[i].jobid, pidnumber);
				}
			}
			prompt();
			
		}
	}
	return;

}

void child_process(char **args)
{
	pid_t pid, wpid;
	int status;
	int i=0;
	int x=0;
	while(args[i]!=NULL)
	{
		if(strcmp(args[i],"&") == 0)
		{	
			args[i] = NULL;
			x = 1;
			break;
		}
		i++;
	}
	//printf("x : %d\n",x );


	pid = fork();
	if (pid == 0) {

	for (i = 0; i < num_builtins(); i++) {
		if (strcmp(args[0], builtin_str[i]) == 0) {
			(*builtin_func[i])(args);
		}
	}
		if(x == 1)
			exit(EXIT_SUCCESS);
	}


	else if (pid < 0) {
		fprintf(stderr,"Error forking\n");
	} 

	else {
		//printf("Parent process\n");
		if(x == 1){
			back_process(pid,args[0]);
	}

	else{
		do {
			wpid = waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
		// Parent process
	}

	background_fxn();

	}
}



void back_process(int pid,char * str){
						backgrund_process[++max].pid = pid;
						backgrund_process[max].state = 1;
						backgrund_process[max].command = str;
						backgrund_process[max].jobid = max;
						printf("The process %s with pid %d has started in the background\n",str,pid);
						printf("[%d] %d\n",backgrund_process[max].jobid,pid);
						return;

}
int lsh_launch(char **args)
{
	//printf("lsh_launch: %s",args[0]);
	pid_t pid, wpid;
	int status;
	int i=0;
	int x=0;
	while(args[i]!=NULL)
	{
		if(strcmp(args[i],"&") == 0)
		{	
			args[i] = NULL;
			x = 1;
			break;
		}
		i++;
	}
	//printf("x : %d\n",x );


	pid = fork();
	if (pid == 0) {
		//printf("Child process\n");
		// Child process
		if (execvp(args[0], args) == -1) {
		//	perror("lsh");
	//	fprintf(stderr,"%s : Command not Found\n",args[0]);
		}
		else{
		if(x == 1)
			exit(EXIT_SUCCESS);
	}
	}
	else if (pid < 0) {
		// Error forking
		fprintf(stderr,"Error forking\n");
		//perror("lsh");
	} 
	else {
		//printf("Parent process\n");

		if(x == 1){
			back_process(pid,args[0]);
									//			signal(SIGCHLD,sigh);
						//prompt();
	}

	else{
		do {
			wpid = waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
		// Parent process
	}

	background_fxn();

	}
	return 1;
}


int lsh_execute(char **args)
{
	int i;

	if (args[0] == NULL) {
		// An empty command was entered.
		return 1;
	}

	for (i = 0; i < num_builtins(); i++) {
		if (strcmp(args[0], builtin_str[i]) == 0) {
			child_process(args);
			return 1;
		}
	}

	return lsh_launch(args);
}
