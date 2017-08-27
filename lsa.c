/*Listing the Files in a directory */
/*
   struct stat {
   dev_t     st_dev;       // ID of device containing file 
ino_t     st_ino;         // inode number 
mode_t    st_mode;        // protection 
nlink_t   st_nlink;       // number of hard links 
uid_t     st_uid;         // user ID of owner 
gid_t     st_gid;         // group ID of owner 
dev_t     st_rdev;        // device ID (if special file) 
off_t     st_size;        // total size, in bytes 
blksize_t st_blksize;     // blocksize for filesystem I/O 
blkcnt_t  st_blocks; 

*/



#include <sys/types.h>
#include <sys/dir.h>
#include <sys/param.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <locale.h>
#include <langinfo.h>
#include <stdint.h>
#include <fcntl.h>

static char perms_buff[30];

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

int main()
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
