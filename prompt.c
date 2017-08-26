#include <stdio.h> 
#include <pwd.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <string.h>
#define TOKEN_BUFFER_SIZE 64
#define TOKEN_DELIM " \t\r\n\a"
#define CMD_DELIM ";"
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
	char **args;
	char * line=read_line();
	args = split_cmd_fxn(line);
	while(args[j]){
		char **args2 = split_line_fxn(args[j]);
		j++;
	}
	prompt();
	//return;
}

int main()
{
	prompt();
	return 0;
}




