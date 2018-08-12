compile: main.o  pwd.o readline.o launch_execute.o cd.o ls.o pinfo.o  echo.o split.o nightswatch.o setenv.o unsetenv.o pipe.o redirect.o
	gcc main.o  pwd.o readline.o launch_execute.o cd.o ls.o pinfo.o  echo.o split.o nightswatch.o setenv.o unsetenv.o pipe.o redirect.o -lncurses 

main.o: main.c
	gcc -c main.c

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

nightswatch.o: nightswatch.c
	gcc -c nightswatch.c

setenv.o: setenv.c
	gcc -c setenv.c

unsetenv.o: unsetenv.c
	gcc -c unsetenv.c

pipe.o: pipe.c
	gcc -c pipe.c

redirect.o: redirect.c
	gcc -c redirect.c

clean: cd.o pwd.o readline.o launch_execute.o ls.o pinfo.o  echo.o split.o nightswatch.o
	rm *.o
