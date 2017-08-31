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
//over

//builtin_func, that’s OK! I am too. It’s an array of function pointers (that take array of strings and return an int). 





////
////

void  SIGINT_handler(int sig)
{
     signal(sig, SIG_IGN);
     printf("\n");	
     signal(sig, SIGINT_handler);
}

void  SIGQUIT_handler(int sig)
{
     signal(sig, SIG_IGN);
     exit(3);
}


void print_prompt(){
	char * name;
	//signal(SIGINT,sigintHandler);
	//signal(SIGTSTP,sigstop);
	struct passwd * result;
	struct passwd pwd;
	char * buf;
	size_t bufsize;
	bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
	if (bufsize == -1)          // Value was indeterminate 
		bufsize = 163;        // Should be more than enough 


	buf = malloc(bufsize);
	int s = getpwuid_r(getuid(),&pwd,buf,bufsize,&result);

	name = result->pw_name;
	printf("<%s@",name);

	//char home[1111] = "/home/";
	//strcat(home, name);
	//	printf("home : %s\n",home);
	char hostname[1111];
	hostname[1110] = '\0';
	gethostname(hostname,1110);
	printf("%s:",hostname);
	char cwd[1111]; //current working directory
	getcwd(cwd,sizeof(cwd));
	//printf("cwd : %s\n",cwd);
	int flag=0;
	int i=0;
	//fflush(stdout);
	char h[100];

	strcpy(h,home);
	
	while(h[i] != '\0')
	{
		//printf("%d",5);
		if(cwd[i] == h[i])
		{
			i++;
		
		}
		else
		{	flag=1;
		break;
		}
	}
	if(flag)
	{
		printf("%s> ",cwd);
	}
	else{
		printf("~");
		while(cwd[i]!='\0')
		{
			printf("%c",cwd[i]);
			i++;
		}
			printf("> ");
	}

}
void prompt()
{
	/*	printf("prompt");
		char *name;
		struct passwd *pass;
		pass = getpwuid(getuid());
		name = pass->pw_name;
		printf("<%s@",name);
		printf("This is the login name: %s\n", name);
		*/
	while(1){
//		if (signal(SIGINT, SIGINT_handler) == SIG_ERR) {
  //       printf("SIGINT install error\n");
	//	exit(1);
//}
		



//						signal(SIGCHLD,sigh);
		

	//check_process();
	
	print_prompt();

			//	fflush(stdout);
	int j=0;
	int k=0;
	char **args;


	char * line=read_line();
	if (feof(stdin)) {
        printf("\n");
                            fflush(stdout);
                           fflush(stderr);
                           exit(0);
                }

	args = split_cmd_fxn(line);
	while(args[j]){
		char **args2 = split_line_fxn(args[j]);
		j++;
		k = lsh_execute(args2);
	}
	background_fxn();
	
	//prompt();
}
	//return;
}
void ctrlZ(int signo)
{
	if(signo == SIGTSTP)
	{
		printf("Detected Ctrl+Z\n");
		print_prompt();
	}

}



int main()
{
	if (signal(SIGINT, SIGINT_handler) == SIG_ERR) {
          printf("SIGINT install error\n");
          exit(1);
     }
     if (signal(SIGQUIT, SIGQUIT_handler) == SIG_ERR) {
          printf("SIGQUIT install error\n");
          exit(2);
     }
     signal(SIGTSTP, ctrlZ);

	getcwd(home,sizeof(home));
	strcpy(previous,home);
	prompt();
	return 0;
}





