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

	static int
one (const struct dirent *unused)
{
	return 1;
}

int file_select(const struct dirent *entry)
{
if(entry->d_name[0]=='.')
	return 0;
else
	return 1;
}
void die(char *msg)
{
	perror(msg);
	exit(0);
}
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
int ls(char ** args)
{		
//	printf("\nentered ls\n");
	// ls -l ls -al ls -la
	int i=1;
	while(args[i] !=NULL)
	{
		if(args[i][0] == '&')
			{
				lsh_launch(args,1);
				return 1;
			}
		i++;

	}

	struct dirent **namelist;
	int n,flag=0;
	if(args[1] && !args[2])
	{

		if(args[1]&&((strcmp(args[1],"-l")==0)||(strcmp(args[1],"-al")==0)||(strcmp(args[1],"-la")==0)))
			return lsl(args);
		else if(strcmp(args[1],"-a")==0)
		{	
			n=scandir(".",&namelist,one,alphasort);
			flag=1;
		}
		//else if(strcmp(args[1],"&")==0)
		//{
		//	lsh_launch(args,1);
		//}
			else
		{
			DIR * curr_dir;
			int flag =0 ;
			char buf[512];

			curr_dir = opendir(args[1]);
		//	printf("%s\n",args[1]);
		//	printf("%s\n",curr_dir);

			struct dirent *myfile;
			int i=0;
			if(curr_dir!=NULL)
			{
				while((myfile = readdir(curr_dir)) !=NULL)
			{
				i++;
				flag = 1;
				char *temp = myfile->d_name;
            	if (temp[0]=='.')
            		continue;
            	sprintf(buf, "%s ",myfile->d_name);
				printf("%s ",buf);
				//if(i%10 == 9)
					printf("\n");

			}
		}
			else
			fprintf(stderr,"ls: cannot access %s: No such file or directory\n",args[1]);
		}
		//else 
		//	fprintf(stderr,"lgdfhs %s : Command Not found\n",args[1]);
	
	}
	else if(args[1]&&args[2]&&!args[3])
	{
	if((strcmp(args[1],"-l")==0&&strcmp(args[2],"-a")==0)||(strcmp(args[2],"-l")==0&&strcmp(args[1],"-a")==0))
	{
		return lsl(args);
	}	
	else
		fprintf(stderr,"ls %s %s : Command Not found\n",args[1],args[2]);
	}
	else if(!args[1])
	{
		n=scandir(".",&namelist,file_select,alphasort);
	flag=1;
	}

		
	
if(flag){
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
			//if(i%10==9)
			printf("\n");
			}
		}
	free(namelist);
	}
return 1;

	}
}
