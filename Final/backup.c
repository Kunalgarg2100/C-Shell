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

int print_prompt(){
	char * name;
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
	printf(ANSI_COLOR_CYAN "<%s"  ANSI_COLOR_RESET,name);
	printf(ANSI_COLOR_YELLOW "@"  ANSI_COLOR_RESET);

	//char home[1111] = "/home/";
	//strcat(home, name);
	//	printf("home : %s\n",home);
	char hostname[1111];
	hostname[1110] = '\0';
	gethostname(hostname,1110);
	printf(ANSI_COLOR_CYAN "%s:" ANSI_COLOR_RESET,hostname);
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
		printf(ANSI_COLOR_CYAN "%s> " ANSI_COLOR_RESET,cwd);
	}
	else{
		printf(  ANSI_COLOR_CYAN "~"   ANSI_COLOR_RESET);
		while(cwd[i]!='\0')
		{
			printf(ANSI_COLOR_CYAN "%c"  ANSI_COLOR_RESET,cwd[i]);
			i++;
		}
			printf(ANSI_COLOR_CYAN "> "  ANSI_COLOR_RESET);
	}
	return 1;

}
void prompt()
{
	while(1)
	{
		//printf("\nhelps\n");
	print_prompt();
	int j=0;
	int k=0;
	char **args;
	char * line=read_line();
	if (feof(stdin)) {
        			printf(" lo l o \n");
                            fflush(stdout);
                           fflush(stderr);
                           exit(0);
                }

	args = split_cmd_fxn(line);
	while(args[j])
	{
		char **args2 = split_line_fxn(args[j]);
		j++;
		k = execute_func(args2);
	}
	//background_fxn();
}
}



int main()
{
	if (signal(SIGINT, SIGINT_handler) == SIG_ERR)
	 {
          printf("SIGINT install error\n");
          exit(1);
     }
   //  else
     //	printf("\nsignal called\n");

    signal(SIGTSTP,SIGTSTP_handler);
	getcwd(home,sizeof(home));
	strcpy(previous,home);
	prompt();
	return 0;
}