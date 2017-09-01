#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <signal.h>
#include "function.h"

void  SIGINT_handler(int signal_num)
{
     	signal(signal_num, SIG_IGN); // The signal is ignored.
		printf("\n");
		print_prompt();
		fflush(stdout); // Flushes the output
     	signal(signal_num, SIGINT_handler); //Again it checks for signal
}

/* 
	Signal  After fork() every child process keeps running same loop as parent, creating another child processes
 	and that's why you end up having a lot of them. The signal is sent to every child process of the current process.
	When you're using fork, you create a child process that inherits from the main process the SIGINT handler.
 	That's why the message is printed several times. So, we need to exit every child process to avoid this error
*/

void SIGTSTP_handler(int signal_num)
{
	if(signal_num == SIGTSTP)
		printf("Ctrl+Z pressed\n");
}
