#include <stdio.h> 
#include <pwd.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <string.h>
char home[1111] = "/home/";
int main() {
	char * name;
	struct passwd * result;
	struct passwd pwd;
	char * buf;
	size_t bufsize;
	bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
	if (bufsize == -1)          /* Value was indeterminate */
		bufsize = 163;        /* Should be more than enough */


	buf = malloc(bufsize);
	int s = getpwuid_r(getuid(),&pwd,buf,bufsize,&result);
	name = result->pw_name;
	printf("<%s@",name);
	strcat(home, name);
//	printf("home : %s\n",home);
	char hostname[1111];
	hostname[1110] = '\0';
	gethostname(hostname,1110);
	printf("%s:",hostname);
	char cwd[1111]; //current working directory
	getcwd(cwd,sizeof(cwd));
//	printf("cwd : %s\n",cwd);

	int flag=0;
	int i=0;
	while(home[i] != '\0')
	{
		if(cwd[i] == home[i])
			i++;
		else
			flag=1;
	}
	if(flag)
		printf("%s>\n",cwd);
	else{
		printf("~");
		while(cwd[i]!='\0')
		{
			printf("%c",cwd[i]);
			i++;
		}
	printf(">");
	}
	return 0;
}



