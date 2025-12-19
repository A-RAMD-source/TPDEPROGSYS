# TPDEPROGSYS
					LAB REPORT OF THE FIRST CLASS.
-----Question 1-----
The objective of this question was to create the entry point for the enseash shell and display a welcome message and the initial prompt without any printfs, we thus programmed a process via a main function with the write() system call : write(STDOUT_FILENO, FIRSTSTR, strlen(FIRSTSTR)); write(STDOUT_FILENO, SECONDSTR, strlen(SECONDSTR));

we directly invoke a kernel system call that writes bytes to file descriptor 1 which ensure immediate output. Also we had a constraint : NO printfs ! This explains why we implemented it that way.
Here the file descriptor is the standard output since we want to display something on the screen which explains the STDOUTFILENO. We took care of declaring with #define the messages wanted. Also, program returns EXIT_SUCCESS, signaling to the parent shell that the process terminated normally.


-----Question 2----- 
We follow the REPL concept to make the enseash mini shell.
For the "Read" part of the REPL, we just have to use the read() system call although this time the file descriptor is the standard INPUT (what is typed in the keyboard typically). For the EXEC part, we had to use fork() because the goal is to successfully run an external command (like fortune or date) and then return immediately to the shell's main loop to accept the next command. Typically : The parent continues running the shell loop, while the child is dedicated to executing the requested command. 
In the child process, execvp() is used and not execv(): execvp(argv_exec[0], argv_exec); that's because with an execv(), we must provide the full, absolute path to the executable thus a simple command such as ls would not be interpreted. We want to type fortune or date without knowing exactly where those programs are stored and execvp() handles this abstraction.

 Also, we only work with simple commands here that's why we implemented the two element array terminating with NULL. We also handled the error case of the execvp indicating that the child process must terminate immediately (with the exit(EXIT_FAILURE)). Although, we had a problem, the commands were NOT executed, even though the code had no syntax errors. That's because The read() system call includes the newline character when the user presses Enter. Thus, to execute the command using execvp, the command name in the buffer must be null-terminated and must not contain \n, and that's why we added in the loop a code that replaces the newline caracter with at the end of the input with a null... and it worked !


-----Question 3-----
We have to create our own exit command whih not only leaves the shell but displays a "Bye bye" message. We also have to create the macro Ctrl+d. The exit command gets easily handled with a comparaison between the buffer and "exit" (via a strcmp) and a read() systemcall. For the macro, we had to think more. Indeed, the Ctrl + D sequence itself does not generate any data, thus the content of the buffer is therefore irrelevant and remains unchanged. But we also know that in the event of an End Of File signal, the read() system call is guaranteed to return 0 thus a logical loop on the value of the readcount is sufficient to handle the macro.


-----Question 4-----
We want to modify the prompt to display the termination status.
The parent shell acquires the encoded exit status of the terminated child process in the 'status'.
Regarding wheter a process terminated normally or not, we'll use a different macro that will extract the value of the status. If the process terminated normally (via exit or return), we use the WIFEXITED(status) macro to confirm this, and then the WEXITSTATUS(status) macro extracts the integer exit code. If, however, the process was terminated by a signal (like a <Ctrl>+C), we use the WIFSIGNALED(status) macro, followed by the WTERMSIG(status) macro to extract the specific signal number. Eventually we format it how we want it to be through snprintf's.

-----Question 5-----
We want to measure the execution time of a command. How ? For that we need the time elapsed between right before the creation of the fork process (to ensure that everything that is child-related is taken into account) and right after the wait() (because the command execution ends when the child process terminates and wait() returns in the parent). We implement that. We declare our startingtime and ending time that are of type struct timespec (time represented with a nanosecond precision). We get the exact time with clock_gettime(CLOCK_REALTIME, &startingtime); and clock_gettime(CLOCK_REALTIME, &endtime); Although we want the execution time of a command in milliseconds so we convert our timestamps in milliseconds. Therefore, we compute the difference between the two timestamps and explicitly convert the result into milliseconds by combining the seconds and nanoseconds fields with this implementation : (endtime.tv_sec - startingtime.tv_sec) * 1000 + (endtime.tv_nsec - startingtime.tv_nsec) / 1000000. This computation is stored elapsedtimeinms = calculate_ms(startingtime, endtime); And since elapsedtimeinms is declared as a long (nanosecond precision) we add the l in the %d to make it %ld. which ensures a good formatting.


-----Question 6-----
Now we want to handle a command with its arguments. The problem here is that the buffer is interpreting a single string so if we write "ls -la ./" he interprets this as a whole command which is not the case. Goal : separate the command from its arguments. So we want to manipulate this string to do such a separation whenever a space is detected so we want to replace the space with a null character to indicate that its the end. That is done with strtok(buffer, " "). Thus we create an array of pointers exec_args such that exec_args0 represents the command and exec_args1 reperesents the argument and that ends with NULL to respect the execvp conditions and we do an execvp(exec_args0, exec_args1). However at first it didn't work, but that's because we didn't code in the son process (in the loop (if (pid == 0))).


-----Question 7-----
The essence of this question is kind of the same as the previous one. If there is a <, respectivelly > in he buffer then the next argument is our input_file, respectivelly, output_file. If it's an input file ( if (input_file != NULL)) we open the file using open(input_file, O_RDONLY), which provides a new file descriptor (in_fd) pointing to that file and similarly, when the > symbol is detected, the following argument is stored in output_file and they are not inserted in the array. This is so that argv_exec cntains only the arguments needed and the name of the command and eventually NULL (argv_exec[argc] = NULL;). If an input file is specified, we open it in read-only mode (int in_fd = open(input_file, O_RDONLY);) and eventually we replace the standard input with the file descriptor of the input file. If it's an output file : The file is opened (or created if it does not exist) in write-only mode, and truncated to ensure that previous contents are not taken in account. The octal value is, by looking at our course, 0600+0400+0004 = 0644. Eventually, we redirect the standard output with our output file : dup2(out_fd, STDOUT_FILENO);
 



