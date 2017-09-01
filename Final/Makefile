compile: backup.o  pwd.o readline.o launch_execute.o cd.o ls.o pinfo.o  echo.o split.o nightswatch.o signal.o
	gcc backup.o  pwd.o readline.o launch_execute.o cd.o ls.o pinfo.o  echo.o split.o nightswatch.o signal.o -lncurses 

backup.o: backup.c
	gcc -c backup.c

cd.o: cd.c
	gcc -c cd.c

pwd.o: pwd.c
	gcc -c pwd.c

readline.o: readline.c
	gcc -c readline.c

launch_execute.o: launch_execute.c
	gcc -c launch_execute.c


ls.o:ls.c
	gcc -c ls.c

pinfo.o: pinfo.c
	gcc -c pinfo.c

echo.o: echo.c
	gcc -c echo.c

split.o: split.c
	gcc -c split.c

signal.o: signal.c
	gcc -c signal.c

nightswatch.o: nightswatch.c
	gcc -c nightswatch.c

clean: os.o cd.o pwd.o readline.o launch_execute.o execute.o ls.o pinfo.o  echo.o exit_shell.o split.o nightswatch.o
	rm *.o

uninstall: os.o cd.o pwd.o readline.o launch_execute.o execute.o ls.o pinfo.o  echo.o exit_shell.o split.o
	rm *
