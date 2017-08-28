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

//over
#define TOKEN_BUFFER_SIZE 64
#define TOKEN_DELIM " \t\r\n\a"
#define CMD_DELIM ";\n"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
int cd(char ** args);
int pwd(char ** args);
int echo(char ** args);
int ls(char ** args);
int pinfo(char ** args);
int lsl(char ** args);
int exit_shell(char ** args);
void sigstop(int sig_num);
void prompt();
static char perms_buff[30];
int sigpid=0;
char home[1111];

char *builtin_str[] = {
	"cd","pwd","echo","ls","exit_shell","pinfo"
};
int (*builtin_func[]) (char **) = {
	&cd,&pwd,&echo,&ls,&exit_shell,&pinfo
};
//builtin_func, that’s OK! I am too. It’s an array of function pointers (that take array of strings and return an int). 

int num_builtins() {
	return sizeof(builtin_str) / sizeof(char *);
}

typedef struct curr_job{
	int pid;
	int state;
	int jobid;
	char * command;
}curr_job;

curr_job backgrund_process[111];
int max = 0;
int get_perms(mode_t mode)
{
	char ftype = '?';

	//all these are check types which return 0 when false and 1 when true here mode is the argument recceive as the value of st_mode of stat struct , it contains information about the permissions and type of file
	if (S_ISREG(mode)) ftype = '-';
	if (S_ISLNK(mode)) ftype = 'l';
	if (S_ISDIR(mode)) ftype = 'd';
	if (S_ISBLK(mode)) ftype = 'b';
	if (S_ISCHR(mode)) ftype = 'c';
	if (S_ISFIFO(mode)) ftype = '|';
	printf("%c",ftype);
	// sprintf stores the output into perms_buff instead of printing on console
	// basicallt mode has bits which on doing and matches a particcular means that it has that permission and therfore would give 1 else 0
		int cnt = 0;
		printf( (mode & S_IRUSR) ? "r" : "-");
        printf( (mode & S_IWUSR) ? "w" : "-");
        printf( (mode & S_IXUSR) ? "x" : "-");
        printf( (mode & S_IRGRP) ? "r" : "-");
        printf( (mode & S_IWGRP) ? "w" : "-");
        printf( (mode & S_IXGRP) ? "x" : "-");
        printf( (mode & S_IROTH) ? "r" : "-");
        printf( (mode & S_IWOTH) ? "w" : "-");
        printf( (mode & S_IXOTH) ? "x" : "-");
        if((mode & S_IRUSR) && (mode & S_IWUSR) && (mode & S_IXUSR) && (mode & S_IRGRP) && (mode & S_IWGRP) && (mode & S_IXGRP)
         && (mode & S_IROTH) && (mode & S_IWOTH) && (mode & S_IWOTH))
        cnt++;
    return cnt;

}
char pathname[MAXPATHLEN];

void die(char *msg)
{
	perror(msg);
	exit(0);
}

	static int
one (const struct dirent *unused)
{
	return 1;
}

int file_select(const struct dirent *entry)
{
//	printf("\nentered file select\n");
if(entry->d_name[0]=='.')
	return 0;
else
{
//	printf("\ntrue also\n");
	return 1;

}
}
int exit_shell(char ** args)
{
exit(EXIT_FAILURE);
}
void sigh(int signum)
{
	pid_t wpid;
	int status, i;
	char pidnumber[1000];
	wpid = waitpid(-1, &status, WNOHANG);
	if(wpid > 0)
	{
		if(WIFEXITED(status) == 0)
		{
			for(i = 1; i <=max; i++)
			{
				if(backgrund_process[i].pid == wpid)
				{
					strcpy(pidnumber, backgrund_process[i].command);
					backgrund_process[i].state = -1;
					printf("\n[%d]	Done\t\t\t%s\n",backgrund_process[i].jobid, pidnumber);
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
					printf("\n[%d]	Done\t\t\t%s\n",backgrund_process[i].jobid, pidnumber);
				}
			}
			prompt();
			
		}
	}
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
		fprintf(stderr,"%s : Command not Found\n",args[0]);
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
	} else {
		//printf("Parent process\n");

		if(x == 1){
						backgrund_process[++max].pid = pid;
						backgrund_process[max].state = 1;
						backgrund_process[max].command = args[0];
						backgrund_process[max].jobid = max;
						printf("[%d] %d\n",backgrund_process[max].jobid,pid);
						signal(SIGCHLD,sigh);
						prompt();
	}
	else{
		sigpid = pid;
		do {
			wpid = waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
		// Parent process
	}
	sigpid = 0;

	}
	return 1;
}



int lsl(char **args){
int count,i;
	struct direct **files;
	struct stat statbuf;
	char datestring[256];
	struct passwd pwent;
	struct passwd *pwentp;
	struct group grp;
	struct group *grpt;
	struct tm time;
	char buf[1024];

	if(!getcwd(pathname, sizeof(pathname)))
		die("Error getting pathnamen");

	if(strcmp(args[1],"-al")==0||strcmp(args[1],"-la")==0||(args[2]&&((strcmp(args[1],"-l")==0&&strcmp(args[2],"-a")==0)||(strcmp(args[2],"-l")==0&&strcmp(args[1],"-a")==0))))
		count = scandir(pathname, &files, one, alphasort);
	else
		count = scandir(pathname, &files, file_select, alphasort);

	if(count > 0)
	{
		printf("total %d\n",count);

		for (i=0; i<count; ++i)
		{
			//stat obtains information of the specified file and writes it to area pointed by &statbuf ;; on success it returns 0 and basically makes a struct for a particular file which contains the members specified at the top of this code
			if (stat(files[i]->d_name, &statbuf) == 0)
			{
				/* Print out type, permissions, and number of links. */
				int cnt = get_perms(statbuf.st_mode);
				printf(" %2d", statbuf.st_nlink);

				if (!getpwuid_r(statbuf.st_uid, &pwent, buf, sizeof(buf), &pwentp))
					printf(" %s", pwent.pw_name);
				// pwname is username
				else
					printf(" %d", statbuf.st_uid);
// userid of owner
				if (!getgrgid_r (statbuf.st_gid, &grp, buf, sizeof(buf), &grpt))
					printf(" %s", grp.gr_name);
				else
					printf(" %d", statbuf.st_gid);

				/* Print size of file. */
				printf(" %7d", (int)statbuf.st_size);

				localtime_r(&statbuf.st_mtime, &time);
				/* Get localized date string. */
				//printf(" %s %d %d:%d %s\n",calender(time.tm_mon),time.tm_mday,time.tm_hour,time.tm_min,files[i]->d_name);
				
				strftime(datestring, sizeof(datestring), "%b %d %H:%M ", &time);

				printf(" %s ", datestring);
				if (S_ISDIR(statbuf.st_mode))
				printf(ANSI_COLOR_BLUE "%s" ANSI_COLOR_RESET "\n",files[i]->d_name);
				else if(cnt)
				printf(ANSI_COLOR_GREEN "%s" ANSI_COLOR_RESET "\n",files[i]->d_name);

				else
				printf("%s\n",files[i]->d_name);

			}

			free (files[i]);
		}

		free(files);
	}

}

int pwd(char ** args)
{
	char cwd[1111];
	getcwd(cwd,sizeof(cwd));
	printf("\n%s\n",cwd);
return 1;
}
int ls(char ** args)
{		
//	printf("\nentered ls\n");
	// ls -l ls -al ls -la
	struct dirent **namelist;
	int n,flag=0;
	if(args[1]&&!args[2])
	{
		if(args[1]&&((strcmp(args[1],"-l")==0)||(strcmp(args[1],"-al")==0)||(strcmp(args[1],"-la")==0)))
			return lsl(args);
		else if(strcmp(args[1],"-a")==0)
		{	n=scandir(".",&namelist,one,alphasort);
		flag=1;
		}
		else
			fprintf(stderr,"ls -%s : Command Not found\n",args[1]);
	}
	else if(args[1]&&args[2]&&!args[3])
	{
	if((strcmp(args[1],"-l")==0&&strcmp(args[2],"-a")==0)||(strcmp(args[2],"-l")==0&&strcmp(args[1],"-a")==0))
	{
		return lsl(args);
	}	
	else
		fprintf(stderr,"ls -%s -%s : Command Not found\n",args[1],args[2]);
	}
	else if(!args[1])
	{n=scandir(".",&namelist,file_select,alphasort);
	flag=1;
	}
		
	else
		{
		fprintf(stderr," Command Not found\n");
		
		}
if(flag){
		if(n<0)
		perror("scandir");
	else
	{
		for(int i=0;i<n;i++)
		{
			printf("%s  ",namelist[i]->d_name);
			free(namelist[i]);
			if(i%10==9)
			printf("\n");

		}
	printf("\n");
	free(namelist);
	}
return 1;

	}
}
int cd(char ** args)
{
	int x=0;
	char  h[1000] ;
	strcpy(h,home);
	

	if(!args[1])
	{
		//printf("%c",*h);
		x = chdir(h);
		//printf("%d",x);
		return 1;
		//prompt();
	}
	else if ((strcmp(args[1],"~")==0) || (strcmp(args[1], "~/")==0))
	{
	//	printf("h :- %s\n",h);
		x =  chdir(h);
	//	printf("%d\n",x);
		return 1;
	}


	//printf("bahar hu main\n");
	
	x = chdir(args[1]);
	
	//printf("%d",x);
	if(x<0)
	{
		printf("bash: cd: %s: No such file or directory\n", args[1]);
		printf("bash: cd: %s: No such file or directory\n", args[1]);
	}

	return 1;
}

int echo(char **args)
{
	int i=1;
	while(args[i]!=NULL){
		printf("%s ",args[i]);
		i++;
	}
	printf("\n");
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
		//	printf("lsh_execute: %s\n",args[0]);
		//	printf("lsh_execute: %s\n",args[1]);
			return (*builtin_func[i])(args);
		}
	}

	return lsh_launch(args);
}

char *read_line(void)
{
	char *line = NULL;
	ssize_t bufsize = 0; // have getline allocate a buffer for us
	getline(&line, &bufsize, stdin);
	return line;
}

char **split_cmd_fxn(char *line)
{
	int buffer_size = TOKEN_BUFFER_SIZE;
	int pos = 0;
	char **token_storage = malloc( buffer_size * sizeof(char*));
	char *token;
	char *saveptr1;

	if (token_storage == NULL) {
		fprintf(stderr, "Memory Allocation error\n");
		exit(EXIT_FAILURE);
	}

	token = strtok_r(line,CMD_DELIM,&saveptr1);
	while (token != NULL) 
	{
		token_storage[pos] = token;
		pos++;

		if (pos >= buffer_size) 
		{
			buffer_size += TOKEN_BUFFER_SIZE;
			token_storage = realloc(token_storage,  buffer_size * sizeof(char*));
			if (token_storage == NULL) {
				fprintf(stderr, "Memory Allocation error\n");
				exit(EXIT_FAILURE);
			}
		}

		token = strtok_r(NULL, CMD_DELIM, &saveptr1);
	}

	token_storage[pos] = NULL;
/*	for (int i = 0; i < pos; ++i)
	{
		printf("%s\n", token_storage[i]);
	}
*/	return token_storage;
}

char **split_line_fxn(char *line)
{
	int buffer_size = TOKEN_BUFFER_SIZE;
	int pos = 0;
	char **token_storage = malloc( buffer_size * sizeof(char*));
	char *token;
	char *saveptr1;

	if (token_storage == NULL) {
		fprintf(stderr, "Memory Allocation error\n");
		exit(EXIT_FAILURE);
	}

	token = strtok_r(line,TOKEN_DELIM,&saveptr1);
	while (token != NULL) 
	{
		token_storage[pos] = token;
		pos++;

		if (pos >= buffer_size) 
		{
			buffer_size += TOKEN_BUFFER_SIZE;
			token_storage = realloc(token_storage,  buffer_size * sizeof(char*));
			if (token_storage == NULL) {
				fprintf(stderr, "Memory Allocation error\n");
				exit(EXIT_FAILURE);
			}
		}

		token = strtok_r(NULL, TOKEN_DELIM, &saveptr1);
	}

	token_storage[pos] = NULL;
/*	for (int i = 0; i < pos; ++i)
	{
		printf("%s\n", token_storage[i]);
	}
*/	return token_storage;
}

int pinfo(char ** args)
{
	char * pinfo_pid;
	char * cmd1, * cmd2 , * cmd3;
	cmd1 = (char *)malloc(1111 * sizeof(char));
	cmd2 = (char *)malloc(1111 * sizeof(char));
	cmd3 = (char *)malloc(1111 * sizeof(char));
	//exec_path = (char *)malloc(1111 * sizeof(char));
	char exec_path[1111];
	//printf("size: %d\n",sizeof(exec_path));
	char ** args1, ** args2;
	if(args[1]!=NULL)
	{
		printf("pid -- %s\n", args[1]);
		sprintf(cmd1 ,"ps -o stat= -p %s",args[1]);
		args1 = split_line_fxn(cmd1);
		sprintf(cmd2 ,"ps -o vsz= -p %s",args[1]);
		args2 = split_line_fxn(cmd2);
		sprintf(cmd3 ,"/proc/%s/exe",args[1]);

	}
	else
	{
		int curr_pid = getpid();
		printf("pid -- %d\n", curr_pid);
		sprintf(cmd1 ,"ps -o stat= -p %d",curr_pid);
		args1 = split_line_fxn(cmd1);
		sprintf(cmd2 ,"ps -o vsz= -p %d",curr_pid);
		args2 = split_line_fxn(cmd2);
		sprintf(cmd3 ,"/proc/%d/exe",curr_pid);
	}

	printf("Process Status -- ");
	fflush(stdout);
	lsh_execute(args1);
	printf("Memory -- ");
	fflush(stdout);
	lsh_execute(args2);
	readlink(cmd3 , exec_path , sizeof(exec_path));
	printf("Executable Path -- %s\n", exec_path);
	return 1;

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
	//check_process();
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


//	fflush(stdout);
	int j=0;
	int k=0;
	char **args;
	char * line=read_line();
	args = split_cmd_fxn(line);
	while(args[j]){
		char **args2 = split_line_fxn(args[j]);
		j++;
		k = lsh_execute(args2);
	}
	
	prompt();
	//return;
}



int main()
{
	getcwd(home,sizeof(home));
	prompt();
	return 0;
}





