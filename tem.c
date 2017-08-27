#include "cd.h"
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
int main()
{
	int x=9;
	char *h = "home/kunal";
	chdir(h);
	printf("%d",x);
	return 1;
}




