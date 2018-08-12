#include <stdlib.h>
#include <stdio.h>
#include "function.h"
int set_env(char ** args){

	if(args[1] == NULL){
		fprintf(stderr, "Usage: set_env <varible> <value>\n");
		return 1;
	}

	if(getenv(args[1])==NULL) //If environment variable is not present
	{
		setenv(args[1],"",1);
	}
	if(args[2]==" ") 
		setenv(args[1],"",1);
	else
	{ 
		setenv(args[1],args[2],1);
	}
	exit(0);
}


/*
Usage : setenv
If name does exist in
       the  environment,  then  its  value is changed to value if overwrite is
       nonzero; if overwrite is zero, then the value of name  is  not  changed

*/