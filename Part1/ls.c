#include <stdio.h> 
#include <dirent.h>
#include <fcntl.h>
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
#include "function.h"

char pathname[MAXPATHLEN];

int get_perms(mode_t mode)
{
	char ftype = '?';

	//mode is the argument received as the value of st_mode of stat struct , which
	// contains information about the permissions and type of file
	/* determining the type of file */
	if (S_ISREG(mode)) ftype = '-';                  //regular file
	else if (S_ISLNK(mode)) ftype = 'l';			 //symbolic link
	else if (S_ISDIR(mode)) ftype = 'd';			// directory
	else if (S_ISBLK(mode)) ftype = 'b';			// block device
	else if (S_ISCHR(mode)) ftype = 'c';			// character device
	else if (S_ISFIFO(mode)) ftype = '|';			// Pipe(FIFO)
	else if (S_ISSOCK(mode)) ftype = 's';			// Socket
	printf("%c",ftype);
	
	// basically st_mode field has bits which on doing and matches a particcular means 
	// that it has that permission and therfore would give 1 else 0
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

int all_files(const struct dirent *allfiles)
{
	return 1;
}

int file_select(const struct dirent *checker)
{
	if(checker->d_name[0]=='.')
		return 0; 
	else
		return 1;
}

int lsl(char **args)
{
	int count,i;
	struct direct **files;
	struct stat statbuf;
	char datestring[256]; // To output formatted month day and time
	struct passwd pwent;
	struct passwd *pwentp;
	struct group grp;
	struct group *grpt;
	struct tm tym;
	char buf[1024];

	if(!getcwd(pathname, sizeof(pathname)))
		perror("Error in pathname");

	if(strcmp(args[1],"-al")==0||strcmp(args[1],"-la")==0||(args[2]&&((strcmp(args[1],"-l")==0&&strcmp(args[2],"-a")==0)||(strcmp(args[2],"-l")==0&&strcmp(args[1],"-a")==0))))
		count = scandir(pathname, &files, all_files, alphasort);
	else
		count = scandir(pathname, &files, file_select, alphasort);

	if(count > 0)
	{
		blkcnt_t  t= 0;
		for (i=0; i<count; ++i)
		{
			if (stat(files[i]->d_name, &statbuf) == 0)
			{
				blkcnt_t p = (statbuf.st_blocks)/2;   //blkcnt_t  st_blocks;  number of 512B blocks allocated 
				t+=p;
			}
		}
		printf("total %ld\n",t );
	}
	if(count > 0)
	{
		for (i=0; i<count; ++i)
		{
			/* Stat obtains information of the specified file and writes it to area pointed by &statbuf ;;
		    on success it returns 0 and basically makes a struct for a particular file which contains the 
			members specified at the top of this code */
			
			if (stat(files[i]->d_name, &statbuf) == 0)
			{
				int cnt = get_perms(statbuf.st_mode); /* Print out type, permissions*/
				printf(" %2lu", statbuf.st_nlink);  /* number of hard links */

				//On success, getpwuid_r() return zero
				if (!getpwuid_r(statbuf.st_uid, &pwent, buf, sizeof(buf), &pwentp))
						printf(" %s", pwent.pw_name);  /* username */
				
				//On success, getgrgid_r() return zero
				if (!getgrgid_r (statbuf.st_gid, &grp, buf, sizeof(buf), &grpt))
					printf(" %s", grp.gr_name); /*group name*/

				/* Print size of file. */
              	printf(" %7ld", statbuf.st_size); 

				/* struct tm *localtime_r(const time_t *timep, struct tm *result);
				 The localtime_r() function converts the calendar time timep to broken-down time representation
				 but stores the data in a user-supplied struct */

				localtime_r(&statbuf.st_mtime, &tym);

				/* strftime - format date and time
				size_t strftime(char *s, size_t max, const char *format, const struct tm *tm);
				The  strftime()  function  formats the broken-down time tm according to the format 
				specification format and places the result in the character array s of size max.
				 %b     The abbreviated month name according to the current locale.
				 %d     The day of the month as a decimal number (range 01 to 31).
				 %H     The hour as a decimal number using a 24-hour clock (range 00 to 23).
				 %M     The minute as a decimal number (range 00 to 59).*/

				strftime(datestring, sizeof(datestring), "%b %d %H:%M ", &tym);
				printf(" %s ", datestring); // datestring contains the formaated string

				if (S_ISDIR(statbuf.st_mode)) // Blue color to the directories
					printf(ANSI_COLOR_BLUE "%s" ANSI_COLOR_RESET "\n",files[i]->d_name);
				else if(cnt)  // Green color is given if user,group and other all have 'rwx' permission
					printf(ANSI_COLOR_GREEN "%s" ANSI_COLOR_RESET "\n",files[i]->d_name);
				else // else file name is printed
					printf("%s\n",files[i]->d_name);
			}
			free (files[i]);
		}
		free(files);
	}
	return 1;
}

int ls(char ** args)
{		
	struct dirent **namelist;
	int n,flag=0;
	
	if(args[1] && !args[2])
	{
		if(args[1]&&((strcmp(args[1],"-l")==0)||(strcmp(args[1],"-al")==0)||(strcmp(args[1],"-la")==0))) // for ls -l, ls -la , ls -al
			return lsl(args);
		
		else if(strcmp(args[1],"-a")==0) // ls -a
		{	
			n=scandir(".",&namelist,all_files,alphasort); // all_files function gives n = no of all files
			flag=1;
		}
		else //ls of the specified directory
		{
			DIR * curr_dir;
			int flag =0 ;
			char buf[512];
			curr_dir = opendir(args[1]);
			struct dirent *myfile;
			int i=0;
			if(curr_dir!=NULL)
			{
				while((myfile = readdir(curr_dir)) !=NULL) //Reading all the files  
				{
					i++;
					flag = 1;
					char *temp = myfile->d_name;
					if (temp[0]=='.') //skipping files which are hidden
						continue;
					sprintf(buf, "%s ",myfile->d_name);
					// sprintf stores the output into buf instead of printing on console
					printf("%s ",buf);
					printf("\n");
				}
			}
			else
				fprintf(stderr,"ls: cannot access %s: No such file or directory\n",args[1]);
		}
	}

	else if(args[1]&&args[2]&&!args[3]) // For ls -l -a ; ls -a -l 
	{
		if((strcmp(args[1],"-l")==0&&strcmp(args[2],"-a")==0)||(strcmp(args[2],"-l")==0&&strcmp(args[1],"-a")==0))
			return lsl(args);
		else
			fprintf(stderr,"ls %s %s : Command Not found\n",args[1],args[2]);
	}

	else if(!args[1]) // For ls command
	{
		n=scandir(".",&namelist,file_select,alphasort);
		flag=1;
	}

	/* Listing all the files of the directory */
	if(flag)
	{
		if(n<0)
			perror("scandir");
		else
		{
			struct stat statbuf;
			for(int i=0;i<n;i++)
			{
				if (stat(namelist[i]->d_name, &statbuf) == 0)
				{
					if (S_ISDIR(statbuf.st_mode))
						printf(ANSI_COLOR_BLUE "%s" ANSI_COLOR_RESET "   ",namelist[i]->d_name);
					else
						printf("%s   ",namelist[i]->d_name);
					free(namelist[i]);
					printf("\n");
				}
			}
			free(namelist);
		}
		return 1;
	}
}
