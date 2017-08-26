
#include <stdio.h> 
#include <pwd.h> 
#include <sys/types.h> 
#include <unistd.h> 
int main() {
	char *name;
	struct passwd *pass;
	pass = getpwuid(getuid()); 
	name = pass->pw_name;
	printf("This is the login name: %s\n", name); 
	return 0;
} 


