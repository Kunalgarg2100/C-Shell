#include <stdio.h> 
#include <dirent.h>
#include <fcntl.h>
#include <pwd.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/dir.h>
#include <sys/param.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/stat.h>
#include <grp.h>
#include <time.h>
#include <locale.h>
#include <langinfo.h>
#include <stdint.h>
int sigpid;
#include "function.h"
// Stores the number of background processes started
int max;

// List of funcyions impemented
char * builtin_str[] = { "cd","pwd","echo","ls","pinfo","nightswatch" };


//It’s an array of function pointers (that take array of strings and return an int). 
int (*builtin_func[]) (char **) = {
	&cd,&pwd,&echo,&ls,&pinfo,&nightswatch
};

// Number of built-in functions i.e no of functions implemented 
int num_builtins() {
	return sizeof(builtin_str) / sizeof(char *);
}

void  SIGINT_handler(int signal_num)
{
	int k=0;
	//printf("%d\n",sigpid );
	if(sigpid){
		if(kill(sigpid,SIGINT))
		{			
			//printf("Error:Can't kill the process %s\n", strerror(errno));
			//fprintf(stderr,"Error:Can't kill the process");
			return;
		}
		k =1;
	}
	if(!k){
		signal(signal_num, SIG_IGN); // The signal is ignored.
		printf("\n");
		print_prompt();
		//printf("form signal\n");
		fflush(stdout); // Flushes the output
		signal(signal_num, SIGINT_handler); //Again it checks for signal
	}
}

/* 
	Signal  After fork() every child process keeps running same loop as parent, creating another child processes
 	and that's why you end up having a lot of them. The signal is sent to every child process of the current process.
	When you're using fork, you create a child process that inherits from the main process the SIGINT handler.
 	That's why the message is printed several times. So, we need to exit every child process to avoid this error
*/

void SIGTSTP_handler(int signal_num)
{
	if(signal_num == SIGTSTP)
		printf("Ctrl+Z pressed\n");
}



// Checks if any of the background process has been terminated ? If yes then prints the message
void background_fxn()
{
	pid_t wpid;
	int status, i;
	char pidnumber[1000];
	/* The waitpid() system call suspends execution of the calling process until a 
	   child specified by pid argument has changed  state.
	   -1 meaning wait for any child process.
	   If status is not NULL, waitpid() store status information in the int to which it points.
	   This integer can  be  inspected with the  WIFEXITED(status) and WIFSIGNALED (status) macros
WNOANG :- return immediately if no child has exited.
waitpid():  on  success,  returns  the  process  ID  of the child whose state has changed;
if WNOHANG was specified and one or more child specified by pid exist, but have not yet changed state, then 0 is returned.
On error, -1 is returned.*/

	while((wpid = waitpid(-1, &status, WNOHANG)) > 0) // Checks for the child processes which have changed there status. 
	{
		if(WIFEXITED(status) == 0)
			//returns true if the child terminated normally, that is, by calling exit(3) or _exit(2), or by returning from main().
		{
			for(i = 1; i <=max; i++)
			{
				if(backgrund_process[i].pid == wpid)
					printf("[%d]+	Done\t\t\t%s with pid %d\n",backgrund_process[i].jobid,  backgrund_process[i].command, wpid);
			}
			prompt();
		}

		if(WIFSIGNALED(status) == 0)
			//returns true if the child process was terminated by a signal.
		{
			for(i = 1; i <=max; i++)
			{
				if(backgrund_process[i].pid == wpid)
					printf("[%d]+	Done\t\t\t%s with pid %d\n",backgrund_process[i].jobid,  backgrund_process[i].command , wpid);
			}
			prompt();
		}
	}
	return;
}

//  Checks if the child process to be started is a foreground process or background process and executs accordingly
void child_process(char **args)
{
	pid_t pid, wpid;
	int status;
	int i=0;
	int x=0; // 
	while(args[i]!=NULL)
	{
		if(strcmp(args[i],"&") == 0)
		{	
			args[i] = NULL;
			x = 1; // If command ends with '&' then x is set 1
			break;
		}
		i++;
	}
/*
 We didn't use fork() here as, purpose of fork() is to create a new process.
 Changing the current directory of the new process won't ever"be inherited back" to its parent.
In pinfo we need the process id of the parent so we don't fork it
*/
	if(strcmp(args[0],"cd")== 0 || strcmp(args[0],"pinfo")== 0)
	{
		for (i = 0; i < num_builtins(); i++) {
			if (strcmp(args[0], builtin_str[i]) == 0) {
				(*builtin_func[i])(args);
				background_fxn();
				return;

			}
		}
	}
	else{

		pid = fork(); // Forking the parent process
		if (pid < 0) 
			fprintf(stderr,"Error forking\n");

		else if (pid == 0)  // Child process
		{
			// In child process the corresponding implemented command is called
			for (i = 0; i < num_builtins(); i++) {
				if (strcmp(args[0], builtin_str[i]) == 0) {
					(*builtin_func[i])(args);
					background_fxn();
					exit(0);
				}
			}

			if(x == 1) // If the process started is a background process then the child process exits
				exit(EXIT_SUCCESS);
		}
		else 
		{
			if(x == 1)  // If the new process to be started was a backgorund process , we update the list of background processes
				back_process(pid,args[0]);

			else //If process is a foreground process
			{ 
				do {
					wpid = waitpid(pid, &status, WUNTRACED);
				} while (!WIFEXITED(status) && !WIFSIGNALED(status));  
				/*
				   Parent process waits till Child for the child whose process ID is equal to the value of pid terminates
				   WUNTRACED :- WUNTRACED allows your parent to be returned from wait()/waitpid() if a child gets stopped 
				   as well as exiting or being killed.*/ 
			}
			background_fxn();


		}
	}
}


// This function adds a new background process 
void back_process(int pid,char * str)
{
	backgrund_process[++max].pid = pid;
	backgrund_process[max].command = str;
	backgrund_process[max].jobid = max;
	printf("The process %s with pid %d has started in the background\n",str,pid);
	printf("[%d] %d\n",backgrund_process[max].jobid,pid);
	return;
}

// Used for the commands which are not implemented in this program
int launch_func(char **args)
{
	pid_t pid, wpid;
	int status;
	int i=0;
	int x=0; // Initialise x = 0 
	while(args[i]!=NULL)
	{
		if(strcmp(args[i],"&") == 0)
		{	
			args[i] = NULL;
			x = 1;  // If command ends with '&' then x is set 1
			break;
		}
		i++;
	}

	pid = fork();
	if (pid < 0) 
		fprintf(stderr,"Error forking\n");
	else if (pid == 0) // Child process
	{
		if (execvp(args[0], args) == -1)
			/*
			   The exec family of functions are ultimately a system call. System calls go straight into the kernel
			   and typically perform a very specific service that only the kernel can do.
			   The  exec() family of functions replaces the current process image with a new process image.
			   The exec() functions return only if an error has occurred. */

			fprintf(stderr,"%s : Command not Found\n",args[0]);
		else 
			//if(x == 1)
			exit(EXIT_SUCCESS);
	}
	else // Parent process
	{
		if(x == 1)
			back_process(pid,args[0]);
		else{
			sigpid = pid;
			do {
				wpid = waitpid(pid, &status, WUNTRACED);
			} while (!WIFEXITED(status) && !WIFSIGNALED(status));
			sigpid = 0;

		}
		background_fxn();
	}
	return 1;
}


int execute_func(char **args)
{
	if (args[0] == NULL) 
		return 1;

	for (int i = 0; i < num_builtins(); i++) 
	{
		if (strcmp(args[0], builtin_str[i]) == 0) 
		{
			child_process(args); //If command is implemented in the C code
			return 1;
		}
		else if(strcmp(args[0], "quit") == 0)
			exit(0);
	}
	return launch_func(args);
}
