#include "enseash.h"
#include "functions.c"
#include <fcntl.h>

int main(int argc, char **argv)
{
    char buffer[MAXLENBUFFER];
    ssize_t read_count;
    pid_t pid;
    int status;
    struct timespec startingtime, endtime;////To stock the results of time measuring for q5.
    long elapsedtimeinms; //To stock the results of time measuring for q5.
//To have the proper formatting of the buffer with the form of exit%
    char prompt_status[MAXSTATUSLEN] = " % "
    display_welcome();

//The LOOP part of the REPL
    while(1){
        write(STDOUT_FILENO, PROMPT_BASE, strlen(PROMPT_BASE));
        write(STDOUT_FILENO, prompt_status, strlen(prompt_status));
        read_count = read(STDIN_FILENO, buffer, MAXLENBUFFER-1);
//To handle the macro ctrl+d lets recall that its not dependant of the
//buffer. We just need the value of return of the command and here it will
//make an exit so an end of file so if the readcount is 0 we exit the loop:
        if (read_count == 0){
            break;
        }
        buffer[read_count] = '\0';
//Initially, this block was missing, causing execution failure.
//We have to replace the ('\n') with the null..
        if (read_count > 0 && buffer[read_count - 1] == '\n') {
            buffer[read_count - 1] = '\0';
        }
        if (strcmp(buffer, EXIT) == 0){
            break;
        }
// We want the execution time of a command. This execution time is just the time elapsed between the fork() process and the wait() so we get the time right before the pid=fork() with a clock_gettime() and right after the wait() and we do the difference in milliseconds. 
        clock_gettime(CLOCK_REALTIME, &startingtime);
        pid = fork(); //Initializing the son process

// We start of by handling the case of an error regarding the pid
        if (pid == -1){
            perror("fork error");
            continue;
        }
// If its the id of son we do the EXEC part of the REPL
        else if (pid == 0){
            // For the QUESTION6 we want to handle commands with their arguments... (ton commentaire complet)
            char *exec_args[MAXLENBUFFER];
            int i = 0;
            char *token = strtok(buffer, " ");
            char *output_file = NULL;
            while (token != NULL) {
            	if (strcmp(token, ">") == 0) {
            		output_file = strtok(NULL, " ");
            		break;
            		}
                exec_args[i] = token; 
                i++;
                token = strtok(NULL, " ");
            }
            exec_args[i] = NULL;
            if (output_file != NULL) {
            int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd == -1) {
            	perror("error");
            	exit(EXIT_FAILURE);
            	}
            dup2(fd, STDOUT_FILENO);
            close(fd);
            }
            execvp(exec_args[0], exec_args);
            char *argv_exec[MAXLENBUFFER];
            int argc = 0;
            char *input_file = NULL;
            char *output_file = NULL;
            //split the buffer into arguments
            char *argument = strtok(buffer, " ");
            while (argument != NULL && argc < MAXLENBUFFER - 1) {
                
                if (strcmp(argument, "<")==0){
                    //input case
                    argument = strtok(NULL, " ");
                    if (argument != NULL) {
                        input_file = argument;
                    }

                } else if (strcmp(argument, ">")==0){
                    //output case
                    argument = strtok(NULL, " ");
                    if (argument != NULL) {
                        output_file = argument;
                    }
                } else {
                    // Normal argument case
                    argv_exec[argc] = argument;
                }
                argc++;
                //get the next argument without going over the previous arguments
                argument = strtok(NULL, " ");
            }
            argv_exec[argc] = NULL; // Null-terminate the argument list

            if (input_file != NULL) {
                //open the input file and redirect stdin
                int in_fd = open(input_file, O_RDONLY);
                if (in_fd < 0) {
                    perror("Input file open error");
                    exit(EXIT_FAILURE);
                }
                dup2(in_fd, STDIN_FILENO);
                close(in_fd);
            }
            if (output_file != NULL) {
                //open the output file and redirect stdout
                int out_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, OCTAL);
                if (out_fd < 0) {
                    perror("Output file open error");
                    exit(EXIT_FAILURE);
                }
                dup2(out_fd, STDOUT_FILENO);
                close(out_fd);
            }
            execvp(argv_exec[0], argv_exec);
            // Failure of the execvp
            perror("Command not found");
            exit(EXIT_FAILURE);
        }
        else{
            wait(&status);
            clock_gettime(CLOCK_REALTIME, &endtime);
            elapsedtimeinms = calculate_ms(startingtime, endtime);
            update_prompt_status(prompt_status, status, elapsedtimeinms);
        }
    }
//Right before terminanting the shell displays the byebye message
    write(STDOUT_FILENO, BYE, strlen(BYE));
    return EXIT_SUCCESS;
}
