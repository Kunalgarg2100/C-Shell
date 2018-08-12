// Install the curses library
sudo apt-get install libncurses5-dev libncursesw5-dev

This is an implementation of a shell with code in C .
* To compile: make 
* To run: ./a.out 
* Clean: make clean 
Incase the prompt doesn't appear after a background process is started/terminated or for any other reason, press ENTER.


Features implemented 
1) A shell prompt of the form: <username@system_name:curr_dir> with path relative to home 
2) Semi-colon (;) separated commands 
3) & to run a process in background 
4) Some built in commands like cd ,echo ,ls , and pwd re-implemented 
5) Type quit to exit the shell 
6) Implements foreground and background processes 
7) Process name and id printed when background process terminates 
8) Type pinfo in the prompt to print: pid, process, status, memory and executable path (on Linux machines) 
9) Ctrl-C to terminate a process 
10) Ctrl-D to exit the shell/denote EOF
11) nightswatch :- Either of these two:
					a) interrupt – print the number of times the CPU(s) has(ve) been interrupted by the keyboard controller
									(i8042 with IRQ 1). There will be a line output to stdout once in every time interval that was specified using -n.
					b) dirty 	 – print the size of the part of the memory which is dirty
 
 **Some bash commands are implemented and the rest are invoked using execvp**

Files in Directory:-

1) main.c           :- contains the main ,prompt ,print_prompt function . main function calls the prompt function ,which further 							handles the futher implementation.

2) ls.c             :- contains ls ,lsl ,get_perms function. It handles ls command .

3) echo.c           :- contains echo function . It handles echo command.

4) cd.c       		:- contains cd function . It handles cd command.

5) function.h 		:- contains all the definitions of functions and the definitions of the constants to be used.

6) pinfo.c    		:- contains pinfo function. It prints the process related info of our shell program.

7) pwd.c      		:- contains pwd function . It handles pwd command.

8) readline.c 		:- contains read_line function . It handles the reading of command from user.

9) split.c    		:- conatains split_line_fxn , split_cmd_fxn function . It splits the commands entered so that they can be executed.

10)launch_execute.c :- contains launch_func ,child process ,background_fxn , back_process, execute_func function.

11)nightswatch.c 	:- contains inp,fun,exen,nightswatch function . It handles nightswatch command (Bonus). 
 