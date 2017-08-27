void fun(struct stat fileStat,char * str)
{

	printf( (S_ISDIR(fileStat.st_mode)) ? "d" : (S_ISLNK(fileStat.st_mode)) ? "l" : "-");
	printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
	printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
	printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
	printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
	printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
	printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
	printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
	printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
	printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");
	printf("\n");

}


struct stat fileStat;
/*On success, zero is returned.  On error, -1 is returned, and errno is set appropriately.*/
if(!stat("Assignment",&fileStat)){
	fun(fileStat,"Directory");
	printf("---------------------------------------\n\n");

}
else
printf("Error in checking permissions of Directory\n");
