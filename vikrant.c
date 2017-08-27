#include<stdio.h>
#include<assert.h>
#include<stdlib.h>
#include<unistd.h> 
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include "functions.h"
#include<signal.h>
#include <fcntl.h>
#include <termios.h>
char *argsbg;
char *builtin_str[] = {
	"quit",
	"pinfo",
	"jobs",
	"killallbg",
	"kjob",
	"fg",
};

typedef struct job_t {              
	int pid;              
	int jid;                
	int state;              
	char *cmdline; 
}job_t;
job_t job_list[100];
int max=0;
pid_t sigpid=0;
int (*builtin_func[]) (char **) = {
	&cd,
	&shellexit,
	&pwd,
	&f_echo,
	&pinfo,
	&list_jobs,
	&killall_bg,
	&kjob,
	&fg,
};
int addjob(pid_t pid)
{

	job_list[++max].pid=pid;
	job_list[max].jid=max;
	job_list[max].state=0;
	job_list[max].cmdline=argsbg;
}
int killall_bg(char **args)
{
	int i=0;
	for(i=1;i<=max;i++)
	{
		if(job_list[i].state==0)
		{
			kill(job_list[i].pid,15);
			job_list[i].state=-1;
		}


	}
	max=0;
	return 1;

}
int kjob(char **args)
{
	if(args[1]==NULL || args[2]==NULL)
	{
		fprintf(stderr, "Error: Input format-> kjob <jobid> <signal>\n");
		return 0;
	}
	int jid=atoi(args[1]);
	if(jid>max)
	{
		fprintf(stderr, "Error job number does not exist\n");
		return 0;
	}

	int sig=atoi(args[2]);
	kill(job_list[jid].pid,sig);
	job_list[jid].state=-1;
	return 1;
}
int fg(char **args)
{
	if(args[1]==NULL)
	{
		fprintf(stderr, "Error: input format-> fg 'jobid'\n");
	}
	int jid=atoi(args[1]);
	printf("%s\n",job_list[jid].cmdline);
	sigpid=job_list[jid].pid;
	int status;
	pid_t wpid;
	job_list[jid].state=1;
	kill(job_list[jid].pid,SIGCONT);
	//printf("%s\n",job_list[jid].cmdline);
	do {
		wpid = waitpid(job_list[jid].pid, &status, WUNTRACED);
	} while(!WIFEXITED(status) && !WIFSIGNALED(status));


	return 1;
}
void sigtstp_handler(int sig) 
{
	int i;
	pid_t pid=0;
	if(sigpid)  // if pid ==0, i.e. no fg job, then return directly
	{
		if(kill(sigpid,SIGTSTP)) 
		{
			fprintf(stderr,"Error:Can't stop the process");
			return ;
		}
		else
		{
			addjob(sigpid);
			fprintf(stdout,"stopped\n");
			prompt();

		}
	}
	//printf("hi\n");
	return;
}
//void pipeHandler(char ** args);
void sigint_handler(int sig)
{

	int i;
	if(sigpid)  // if pid ==0, i.e. no fg job, then return directly
	{
		if(kill(sigpid,SIGINT))
			fprintf(stderr,"Error:Can't kill the process");
		else
		{

			printf("\n");

		}
	}
	prompt();
	return;
}
int list_jobs(char **args)
{
	int i=1;
	for(i=1;i<=max;i++)
	{
		if(job_list[i].state==0)
			printf("[%d] %s with PID [%d]\n",job_list[i].jid,job_list[i].cmdline,job_list[i].pid);

	}
	return 1;

}

int launch(char **args)
{
	pid_t pid, wpid;
	int status;
	int i=0,x=0;
	char s[]={"&"};

	while(args[i]!=NULL)
	{
		if(strcmp(args[i],&s[0])==0)
		{
			//	printf("hi\n");
			args[i]=NULL;
			x=1;
			break;
		}
		i++;
	}
	pid = fork();
	if (pid == 0) {
		// Child process
		if (execvp(args[0], args) == -1) {
			perror("lsh");
		}
		exit(EXIT_FAILURE);
	} 
	else if (pid < 0) 
	{
		// Error forking
		perror("lsh");
	} 
	else {
		argsbg=args[0];
		if(x==1)
		{
			//addjob(pid);
			job_list[++max].pid=pid;
			job_list[max].jid=max;
			job_list[max].state=0;
			job_list[max].cmdline=args[0];

			printf("PID is %d\n",pid);
			//printf("%d\n",sigpid);
			prompt();
		}
		else
		{
			sigpid=pid;
			do {
				wpid = waitpid(pid, &status, WUNTRACED);
			} while(!WIFEXITED(status) && !WIFSIGNALED(status));
		}
		sigpid=0;
	}

	return 1;
}


void prompt()
{
	signal(SIGINT,sigint_handler);
	signal(SIGTSTP,sigtstp_handler);
}
