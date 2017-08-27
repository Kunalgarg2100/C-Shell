#include <stdio.h> 
#include <pwd.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <string.h>
#define TOKEN_BUFFER_SIZE 64
#define TOKEN_DELIM " \t\r\n\a"
#define CMD_DELIM ";"
int cd(char ** args);
char *builtin_str[] = {
	"cd",
};
int (*builtin_func[]) (char **) = {
	&cd,
};
//builtin_func, that’s OK! I am too. It’s an array of function pointers (that take array of strings and return an int). 

int num_builtins() {
	return sizeof(builtin_str) / sizeof(char *);
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
int cd(char ** args)
{
	printf("cd :- %s",args[1]);
	int x;
	char * h = "/home/kunal";
	if(!args[1]){
		//printf("%c",*h);
		x = chdir(h);
		printf("%d",x);
		return 1;
		//prompt();
	}
	else if ((strcmp(args[1], "~")==0) || (strcmp(args[1], "~/")==0))
	{
		x =  chdir(h);
		printf("%d",x);
		return 1;
	}


	x = chdir(args[1]);
	printf("%d",x);
	if(x<0)
	{
		printf("bash: cd: %s: No such file or directory\n", args[1]);
		printf("bash: cd: %s: No such file or directory\n", args[1]);
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
			printf("lsh_execute: %s\n",args[0]);
			printf("lsh_execute: %s\n",args[1]);
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
	for (int i = 0; i < pos; ++i)
	{
		printf("%s\n", token_storage[i]);
	}
	return token_storage;
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
	for (int i = 0; i < pos; ++i)
	{
		printf("%s\n", token_storage[i]);
	}
	return token_storage;
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

	char home[1111] = "/home/";
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
		printf("%s> ",cwd);
	else{
		printf("~");
		while(cwd[i]!='\0')
		{
			printf("%c",cwd[i]);
			i++;
		}
		printf("> ");
	}
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
	prompt();
	return 0;
}




