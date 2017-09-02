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
	FD_ZERO(&rfds);
	FD_SET(0, &rfds);

	/* Wait up to five seconds. */
	tv.tv_sec = timer;
	tv.tv_usec = 0;

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
		//printf("\n lololo\n");
		//initscr();
		//timeout(-1);
		char inpu=getch();
		//endwin();
		// printf("inpu = %c\n",inpu );

		//scanf("%c",&inpu);
		//fgets(buff,3, stdin);

		/* Remove trailing newline character from the input buffer if needed. */
		if(inpu=='q')
		{
			endwin();
			return 0;
		}
		//len = strlen(buff) - 1;
		//if (buff[len] == '\n')
		//  buff[len] = '\0';

		//printf("'%s' was read from stdin.\n", buff);
		//printf("Data is available now.\n");
		/* FD_ISSET(0, &rfds) will be true. */}
	//else
	//printf("No data within five seconds.\n");
	return 1;
	//   exit(EXIT_SUCCESS);
}
int var;


int fun(int k,char **args)
{
	int ch ,timer;
	if( k == 1)
	{
		ch = 1;
		timer=2;
	}
	else
	{
		ch = 3;
		timer=atoi(args[2]);
	}

	while(1)
	{
		exen(ch,timer,args);	
		int x=inp(timer);
		//	printf("%d",x);
		if(x==0)break;

	}
	return 1;
}
int exen(int ch,int timer,char **args)	
{	
	FILE *fp;
	char str[11111];
	if(strcmp(args[ch],"dirty") == 0)
	{
		fp = fopen("/proc/meminfo", "r");
		if(!var){
			if(ch == 1){
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


		//char str[11111];
		while(fgets(str, 10000, fp))
		{
			if(strstr(str, "i8042"))
			{
				int x= strlen(str);
				int j;
				for(j=0;j<x;j++)
					if((int)str[j] > 47 && (int)str[j] < 57) //checks first digit
						break;
				if(str[j] == '1' && str[j+1] == ':')
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


int nightswatch(char **args){
	int i=0;
	var=0;
	while(args[i] !=NULL)
	{
		i++;
	}
	if(i == 2){
		if((strcmp(args[1],"dirty")!=0) && (strcmp(args[1],"interrupt")!=0))
		{
			fprintf(stderr, "Argument not supported\n");
			exit(0);
		}
	}
	else if(i == 4){
		if((strcmp(args[3],"dirty")!=0) && (strcmp(args[3],"interrupt")!=0))
		{
			fprintf(stderr, "Argument not supported\n");
			exit(0);
		}
		if(strcmp(args[1],"-n")!=0)
		{		
				fprintf(stderr, "nightswatch: %s: invalid option\n",args[1]);
			exit(0);
		}

	}
	erase();
	initscr();
	refresh();
	if(i == 2)
	{
		fun(1,args);
	}
	else if(i == 4)
	{
		fun(4,args);
	}
	else
	{
		printf("Incorrect command\n");
	}
	exit(0);
}
