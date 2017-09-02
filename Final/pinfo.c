#include <stdio.h> 
#include <stdlib.h>
#include <sys/types.h> 
#include <unistd.h> 
#include <signal.h>

#include "function.h"
/*
   ps displays information about a selection of the active processes.
   --pid pidlist : Selects the processes by process ID
   -o format
   User-defined format. format is a single argument in the form of a blank-separated or comma-separated list, which offers a
   way to specify individual output columns.  

vsz : virtual memory size of the process in KiB (1024-byte units).  Device mappings are currently excluded; this is
subject to change.  (alias vsize).
man ps for more
D    uninterruptible sleep (usually IO)
R    running or runnable (on run queue)
S    interruptible sleep (waiting for an event to complete)
T    stopped by job control signal
t    stopped by debugger during the tracing
W    paging (not valid since the 2.6.xx kernel)
X    dead (should never be seen)
Z    defunct ("zombie") process, terminated but not reaped by its parent
+    is in the foreground process group */

/*
   int kill(pid_t pid, int sig);
   The kill() system call can be used to send any signal to any process group or process.
   If sig is 0, then no signal is sent, but error checking is still performed; this can be used to 
   check for the existence of a process ID or process group ID.
   On success zero is returned.  On error, -1 is returned
   */

int pinfo(char ** args)
{
	char * pinfo_pid;
	char * cmd1, * cmd2 , * cmd3;
	cmd1 = (char *)malloc(1111 * sizeof(char));
	cmd2 = (char *)malloc(1111 * sizeof(char));
	cmd3 = (char *)malloc(1111 * sizeof(char));
	char exec_path[1111];
	char ** args1, ** args2;
	int curr_pid;
	if(args[1]!=NULL){
		curr_pid = atoi(args[1]);
		if(kill(curr_pid,0))
		{
			fprintf(stderr, "Invalid PID\n");
			return 1;
		}

	}
	else
		curr_pid = getpid(); // As pinfo is not a forked process its pid is same as the parent i.e .out

	printf("pid -- %d\n", curr_pid);
	sprintf(cmd1 ,"ps -o stat= -p %d",curr_pid);
	args1 = split_line_fxn(cmd1);
	sprintf(cmd2 ,"ps -o vsz= -p %d",curr_pid);
	args2 = split_line_fxn(cmd2);
	sprintf(cmd3 ,"/proc/%d/exe",curr_pid);
	printf("Process Status -- ");
	fflush(stdout);
	execute_func(args1);
	printf("Memory -- ");
	fflush(stdout);
	execute_func(args2);
	int p =readlink(cmd3 , exec_path , sizeof(exec_path));
	exec_path[p] = '\0';
	printf("Executable Path -- %s\n", exec_path);
	return 1;
}
