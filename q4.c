#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>

//Magic numbers are forbidden... so are the strings!

#define FIRSTSTR "Bienvenue dans le Shell ENSEA.\n"
#define SECONDSTR "Pour quitter, tapez 'exit'.\n"
#define PROMPT_BASE "enseash " 
#define MAXLENBUFFER 512
//We add 2 strings for question 3
#define BYE "Bye bye...\n"
#define EXIT "exit"
//We add a maximal status size
#define MAXSTATUSLEN 128

int main(int argc, char **argv)
{
    char buffer[MAXLENBUFFER];
    ssize_t read_count;
    pid_t pid;
    int status;
//To have the proper formatting of the buffer with the form of exit%
    char prompt_status[MAXSTATUSLEN] = " % ";
    write(STDOUT_FILENO, FIRSTSTR, strlen(FIRSTSTR));
    write(STDOUT_FILENO, SECONDSTR, strlen(SECONDSTR));

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

    pid = fork(); //Initializing the son process

// We start of by handling the case of an error regarding the pid
    if (pid == -1){
        perror("fork error");
        continue;
    }
// If its the id of son we do the EXEC part of the REPL
    else if (pid == 0){
        char *argv_exec[] = {buffer, NULL};
        execvp(buffer, argv_exec);
        // Failure of the execvp
        perror("Command not found");
        exit(EXIT_FAILURE);
    }
    else{
        wait(&status);
	 if (WIFEXITED(status)) {
            int exitcode = WEXITSTATUS(status);
            snprintf(prompt_status, MAXSTATUSLEN, "[exit:%d] %% ", exitcode);
        } else if (WIFSIGNALED(status)) {
            int exitsig = WTERMSIG(status);
            snprintf(prompt_status, MAXSTATUSLEN, "[sign:%d] %% ", exitsig);
        }
    }
}
//Right before terminanting the shell displays the byebye message
    write(STDOUT_FILENO, BYE, strlen(BYE));
    return EXIT_SUCCESS;
}
