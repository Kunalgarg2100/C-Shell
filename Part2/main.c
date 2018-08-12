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
#include <sys/stat.h>
#include <grp.h>
#include <time.h>
#include <locale.h>
#include <langinfo.h>
#include <stdint.h>

#include "function.h"
/*

getuid() returns the real user ID of the calling process.

int getpwuid_r(uid_t uid, struct passwd *pwd, char *buf, size_t buflen, struct passwd **result);
The getpwuid_r() function stores the retrieved passwd structure containing the broken-out fields of the record 
in the password database that matches the user ID uid in the space pointed to by pwd. 
The string fields pointed to by the members of the passwd structure  are  stored  in  the
buffer  buf  of size buflen.  A pointer to the result (in case of success) or NULL is stored in *result.
On success, getpwuid_r() return zero, and set *result to pwd.
The call sysconf(_SC_GETPW_R_SIZE_MAX) returns either -1 or an initial suggested size for buf.

gethostname() returns the null-terminated hostname in the character array name, which has a length of len bytes.
Hostname is used to display the system's DNS name, and to display or set its hostname or NIS domain name.
*/

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

	char hostname[1111];
	hostname[1110] = '\0';
	gethostname(hostname,1110);
	printf(ANSI_COLOR_CYAN "%s:" ANSI_COLOR_RESET,hostname);

	char cwd[1111]; //current working directory
	getcwd(cwd,sizeof(cwd));
	int flag=0;
	int i=0;
	char h[100];

	strcpy(h,home);

	while(h[i] != '\0')
	{
		if(cwd[i] == h[i])
			i++;
		else
		{
			flag=1;
			break;
		}
	}
	if(flag)
		printf(ANSI_COLOR_CYAN "%s> " ANSI_COLOR_RESET,cwd);
	else
	{
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
		print_prompt();

		int j=0;
		int k=0;
		char **args;
		char * line=read_line();
		if (feof(stdin)) //The  function  feof() tests the end-of-file indicator for the stream pointed to by stream,
		// returning nonzero if it is set.
		{
			printf("Ctrl+ D\n");
			fflush(stdout);
			fflush(stderr);
			exit(0);
		}
	
	// Used to separate commands
		args = split_cmd_fxn(line);

		while(args[j])
		{

	// Used to separate command argument
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

	//	printf("%d\n",getpid() );
	signal(SIGTSTP,SIGTSTP_handler);
	getcwd(home,sizeof(home));
	strcpy(previous,home);
	prompt();
	return 0;
}
