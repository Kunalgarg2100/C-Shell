#include <dirent.h>
#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
int file_select(const struct dirent *entry)
{
	if(entry-> d_name[0] == '.')
		return 0;
	else
		return 1;
}
int main(void)
{
	struct dirent **namelist;
	int n;

	n = scandir(".", &namelist,file_select, alphasort);
//	n = scandir(".", &namelist,NULL, alphasort);
	if (n < 0)
		perror("scandir");
	else {
		for(int i=0;i<n;i++)
		{
			printf("%s ", namelist[i]->d_name);
			free(namelist[i]);
			if(i%10 == 9)
				printf("\n");
		}
		printf("\n");
		free(namelist);
	}
}




