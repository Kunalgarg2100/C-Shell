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
int lsh_execute(char **args);
int lsh_launch(char **args);
void sigh(int signum);
char **split_cmd_fxn(char *line);
char **split_line_fxn(char *line);
char *read_line(void);

static char perms_buff[30];
char home[1111];

typedef struct curr_job{
	int pid;
	int state;
	int jobid;
	char * command;
}curr_job;

curr_job backgrund_process[111];
