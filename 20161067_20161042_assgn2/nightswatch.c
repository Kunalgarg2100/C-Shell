#include <curses.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
int exen(int ch,int timer,char **args);

int inp(int timer)
{
	fd_set rfds;
	struct timeval tv;
	int retval,len;
	char buff[100]={0};

	/* Watch stdin (fd 0) to see when it has input. */
	FD_ZERO(&rfds); // clears a set
	FD_SET(0, &rfds); // adds a given file descriptor from a given set

	/* Wait up to timer seconds. */
	tv.tv_sec = timer;
	tv.tv_usec = 0;

/*
int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds ,  struct timeval *timeout);

select() allows a program to monitor multiple file descriptors, waiting until one or more of the file descriptors 
become "ready" for some class of I/O operation (e.g., input possible)
The file descriptors listed in readfds will be watched to see if characters become available for reading
: in writefds will be watched if space is available for write 
: in exceptfds will be watched for exceptional conditions
: The timeout argument specifies the interval that select() should block waiting for a file descriptor to become ready
*/

	retval = select(1, &rfds, NULL, NULL, &tv);
	/* Don't rely on the value of tv now! */

	if (retval == -1)
	{
		perror("select()");
		endwin();
		return 0;
	}
	else if (retval)
	{
		char inpu=getch();
		if(inpu=='q')
		{
			/*
			   A program should always call endwin before exiting or escaping from curses mode temporarily.
			   This routine restores tty modes, moves the cursor to the lower left-hand corner of the screen
			   and resets the terminal into the proper non-visual mode. 
			   The delscreen routine frees storage associated with the SCREEN data structure. The endwin routine does not do this, 
			   so delscreen should be called after endwin if a particular SCREEN is no longer needed. 
			   */
			endwin();
			return 0;
		}

	}
	
	return 1;
}
int var;


int fun(int k,char **args)
{
	int ch ,timer;
	if( k == 1)
	{
		ch = 1;
		timer=2; // If function has 2 arguments 
	}
	else
	{
		ch = 3;
		timer=atoi(args[2]); // timer is let to given argument
	}

	while(1)
	{
		exen(ch,timer,args);	// executes the command
		int x= inp(timer);
		if(x==0)
			break;

	}
	return 1;
}

/*
The printw() function writes to an "imaginary" screen. It puts stuff into a buffer and updates some flags and
does some other internal ncurses bookkeeping. It doesn't actually write anything to your real screen (the console window).
You can do as much printw() writing as you want to, but the stuff doesn't show up on the real screen until your program does 
something else to cause the "imaginary" screen buffer contents to go to the real screen.
One thing that causes the real screen to be updated from the printw() buffer is refresh() 
characters entered in response to getch() are echoed immediately. They don't go to the "printw() buffer", and your program doesn't need refresh(). 
there may be programs where you do some printw() stuff and then go off and to something else (not console input). A common mistake is for people to get into the habit of 
not using refresh()
*/

int exen(int ch,int timer,char **args)	
{	
	FILE *fp;
	char str[11111];
	if(strcmp(args[ch],"dirty") == 0)
	{
		fp = fopen("/proc/meminfo", "r");
		if(!var){
			if(ch == 1)
			{
				printw("Every 2.0s: %s\n\n\n", args[ch]);
				refresh();
			}
			if(ch == 3){
				printw("Every %d.0s: %s\n\n\n", atoi(args[2]),args[ch]);
				refresh();	
			}
			var=1;
		}
		while(fgets(str, 10000, fp))
		{
			if(strstr(str, "Dirty")){

				int x= strlen(str);
				int j;
				for(j=0;j<x;j++)
					if((int)str[j] > 47 && (int)str[j] < 57)
						break;
				for(int k=j;k<x;k++)
				{
					printw("%c",str[k]);
					refresh();
				}
			}
		}

	}

	else if(strcmp(args[ch],"interrupt") == 0)
	{


		fp = fopen("/proc/interrupts", "r");
		fgets(str, 10000, fp);
		if(!var){
			if(ch == 1){
				printw("Every 2.0s: %s\n\n\n", args[ch]);
				refresh();
			}
			if(ch == 3){
				printw("Every %d.0s: %s\n\n\n", atoi(args[2]),args[ch]);
				refresh();	
			}
			for(int p=10;p<strlen(str);p++)
			{	
				var = 1;
				printw("%c", str[p]);
				refresh();
			}
		}


		while(fgets(str, 10000, fp))
		{
			if(strstr(str, "i8042"))
			{
				int x= strlen(str);
				int j;
				for(j=0;j<x;j++)
					if((int)str[j] > 47 && (int)str[j] < 57) //checks first digit
						break;
				if(str[j] == '1' && str[j+1] == ':') //1:
				{
					int k;
					for(k=j+2;k<x;k++)
						if((int)str[k] > 47 && (int)str[k] < 57)
							break;
					for(int l=k;l<x;l++)
					{
						if(str[l] > 57)
							break;
						printw("%c",str[l]);
						refresh();
					}
					refresh();
					printw("\n");
					refresh();
				}

			}
		}
	}
	else
	{
		endwin();
		exit(0);
	}
	return 0;

}


int nightswatch(char **args)
{
	int i=0;
	var=0;

	/* Calculating the no of arguments*/
	while(args[i] !=NULL)
		i++;

	/* If number of arguments are 2 and the second argument should be "dirty" or "interrupt" */
	if(i == 2)
	{
		if((strcmp(args[1],"dirty")!=0) && (strcmp(args[1],"interrupt")!=0))
		{
			fprintf(stderr, "Argument not supported\n");
			exit(0);
		}
	}

	/* If number of arguments are 4 and the fourth argument should be "dirty" or "interrupt" */
	else if(i == 4)
	{
		if((strcmp(args[3],"dirty")!=0) && (strcmp(args[3],"interrupt")!=0))
		{
			fprintf(stderr, "Argument not supported\n");
			exit(0);
		}

		/* 2nd argument should be -n */
		if(strcmp(args[1],"-n")!=0)
		{		
			fprintf(stderr, "nightswatch: %s: invalid option\n",args[1]);
			exit(0);
		}
	}

	erase();
	/* The erase and werase routines copy blanks to every position in the window, clearing the screen.*/

	initscr();
	/*
	   initscr is normally the first curses routine to call when initializing a program. initscr also causes the first call
	   to refresh to clear the screen. If errors occur, initscr writes an appropriate error message to standard error 
	   and exits; otherwise, a pointer is returned to stdscr. */

	refresh();
	/*
	   The refresh and wrefresh routines (or wnoutrefresh and doupdate) must be called to get actual output to the terminal, 
	   as other routines merely manipulate data structures. */

	if(i == 2)
		fun(1,args);
	else if(i == 4)
		fun(4,args);
	else
		printf("Incorrect command\n");
	exit(0);
}
