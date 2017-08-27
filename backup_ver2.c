#include <stdio.h> 
#include<dirent.h>
#include<fcntl.h>
#include <pwd.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <string.h>
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
int cd(char ** args);
int pwd(char ** args);
int echo(char ** args);
int ls(char ** args);
int lsa(char ** args);

static char perms_buff[30];

char home[1111];

char *builtin_str[] = {
	"cd","pwd","echo","ls","lsa",
};
int (*builtin_func[]) (char **) = {
	&cd,&pwd,&echo,&ls,&lsa
};
//builtin_func, that’s OK! I am too. It’s an array of function pointers (that take array of strings and return an int). 

int num_builtins() {
	return sizeof(builtin_str) / sizeof(char *);
}


const char *get_perms(mode_t mode)
{
	char ftype = '?';

	//all these are check types which return 0 when false and 1 when true here mode is the argument recceive as the value of st_mode of stat struct , it contains information about the permissions and type of file
	if (S_ISREG(mode)) ftype = '-';
	if (S_ISLNK(mode)) ftype = 'l';
	if (S_ISDIR(mode)) ftype = 'd';
	if (S_ISBLK(mode)) ftype = 'b';
	if (S_ISCHR(mode)) ftype = 'c';
	if (S_ISFIFO(mode)) ftype = '|';
	// sprintf stores the output into perms_buff instead of printing on console
	// basicallt mode has bits which on doing and matches a particcular means that it has that permission and therfore would give 1 else 0
	sprintf(perms_buff, "%c%c%c%c%c%c%c%c%c%c %c%c%c", ftype,
			mode & S_IRUSR ? 'r' : '-',
			mode & S_IWUSR ? 'w' : '-',
			mode & S_IXUSR ? 'x' : '-',
			mode & S_IRGRP ? 'r' : '-',
			mode & S_IWGRP ? 'w' : '-',
			mode & S_IXGRP ? 'x' : '-',
			mode & S_IROTH ? 'r' : '-',
			mode & S_IWOTH ? 'w' : '-',
			mode & S_IXOTH ? 'x' : '-',
			mode & S_ISUID ? 'U' : '-',
			mode & S_ISGID ? 'G' : '-',
			mode & S_ISVTX ? 'S' : '-');

	return (const char *)perms_buff;
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
int lsh_launch(char **args)
{
	printf("lsh_launch: %s",args[0]);
	pid_t pid, wpid;
	int status;

	pid = fork();
	if (pid == 0) {
		// Child process
		if (execvp(args[0], args) == -1) {
			perror("lsh");
		}
		exit(EXIT_FAILURE);
	} else if (pid < 0) {
		// Error forking
		perror("lsh");
	} else {
		// Parent process
		do {
			wpid = waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	return 1;
}

int lsa(char **args)
{
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

	count = scandir(pathname, &files, one, alphasort);

	if(count > 0)
	{
		printf("total %d\n",count);

		for (i=0; i<count; ++i)
		{
			//stat obtains information of the specified file and writes it to area pointed by &statbuf ;; on success it returns 0 and basically makes a struct for a particular file which contains the members specified at the top of this code
			if (stat(files[i]->d_name, &statbuf) == 0)
			{
				/* Print out type, permissions, and number of links. */
				printf("%10.10s", get_perms(statbuf.st_mode));
				printf(" %d", statbuf.st_nlink);

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
				printf(" %5d", (int)statbuf.st_size);

				localtime_r(&statbuf.st_mtime, &time);
				/* Get localized date string. */
				strftime(datestring, sizeof(datestring), "%F %T", &time);

				printf(" %s %s\n", datestring, files[i]->d_name);
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
	
	struct dirent **namelist;
	
	int n;
		n=scandir(".",&namelist,file_select,alphasort);
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
		printf("h :- %s\n",h);
		x =  chdir(h);
		printf("%d\n",x);
		return 1;
	}


	//printf("bahar hu main\n");
	
	x = chdir(args[1]);
	
	printf("%d",x);
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





