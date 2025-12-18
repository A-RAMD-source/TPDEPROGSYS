#include "enseash.h"
#include "functions.c"

int main(int argc, char **argv)
{
    char buffer[MAXLENBUFFER];
    ssize_t read_count;
    pid_t pid;
    int status;
    struct timespec startingtime, endtime;////To stock the results of time measuring for q5.
    long elapsedtimeinms; //To stock the results of time measuring for q5.
//To have the proper formatting of the buffer with the form of exit%
    char prompt_status[MAXSTATUSLEN] = " % ";
    
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
            char *argv_exec[MAXLENBUFFER];
            int argc = 0;
            //split the buffer into arguments
            char *argument = strtok(buffer, " ");
            while (argument != NULL && argc < MAXLENBUFFER - 1) {
                argv_exec[argc] = argument;
                argc++;
                //get the next argument without going over the previous arguments
                argument = strtok(NULL, " ");
            }
            argv_exec[argc] = NULL; // Null-terminate the argument list
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
