# TPDEPROGSYS
					LAB REPORT OF THE FIRST CLASS.
-----Question 1-----
The objective of this question was to create the entry point for the enseash shell and display a welcome message and the initial prompt without any printfs, we thus programmed a process via a main function with the write() system call. Here the file descriptor is the standard output since we want to display something on the screen which explains the STDOUTFILENO. We took care of declaring with #define the messages wanted. Also, program returns EXIT_SUCCESS, signaling to the parent shell that the process terminated normally.


-----Question 2----- 
We follow the REPL concept to make the enseash mini shell.
For the "Read" part of the REPL, we just have to use the read() system call although this time the file descriptor is the standard INPUT (what is typed in the keyboard typically). For the EXEC part, we had to use fork() because the goal is to successfully run an external command (like fortune or date) and then return immediately to the shell's main loop to accept the next command. Also, we only work with simple commands here that's why we implemented the two element array terminating with NULL. We also handled the error case of the execvp indicating that the child process must terminate immediately (with the exit(EXIT_FAILURE)). Although, we had a problem, the commands were NOT executed, even though the code had no syntax errors. That's because The read() system call includes the newline character when the user presses Enter. Thus, to execute the command using execvp, the command name in the buffer must be null-terminated and must not contain \n, and that's why we added in the loop a code that replaces the newline caracter with at the end of the input with a null... and it worked !


-----Question 3-----
We have to create our own exit command whih not only leaves the shell but displays a "Bye bye" message. We also have to create the macro Ctrl+d. The exit command gets easily handled with a comparaison between the buffer and "exit" (via a strcmp) and a read() systemcall. For the macro, we had to think more. Indeed, the Ctrl + D sequence itself does not generate any data, thus the content of the buffer is therefore irrelevant and remains unchanged. But we also know that in the event of an End Of File signal, the read() system call is guaranteed to return 0 thus a logical loop on the value of the readcount is sufficient to handle the macro.


-----Question 4-----
We want to modify the prompt to display the termination status.
The parent shell acquires the encoded exit status of the terminated child process in the 'status'.
Regarding wheter a process terminated normally or not, we'll use a different macro that will extract the value of the status. If the process terminated normally (via exit or return), we use the WIFEXITED(status) macro to confirm this, and then the WEXITSTATUS(status) macro extracts the integer exit code. If, however, the process was terminated by a signal (like a <Ctrl>+C), we use the WIFSIGNALED(status) macro, followed by the WTERMSIG(status) macro to extract the specific signal number. Eventually we format it how we want it to be through snprintf's.

-----Question 5-----
