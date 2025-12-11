#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>

//Magic numbers are forbidden... so are the strings!

#define FIRSTSTR "Bienvenue dans le Shell ENSEA.\n"
#define SECONDSTR "Pour quitter, tapez 'exit'.\n"
#define P "enseash % "
#define MAXLENBUFFER 512

int main(int argc, char **argv)
{
    // LIGNES AJOUTÉES/CORRIGÉES ICI :
    char buffer[MAXLENBUFFER]; // Correction: Suppression du '=' en trop.
    ssize_t read_count;        // AJOUTÉ : Variable pour le retour de read()
    pid_t pid;
    int status;
    
    write(STDOUT_FILENO, FIRSTSTR, strlen(FIRSTSTR));
    write(STDOUT_FILENO, SECONDSTR, strlen(SECONDSTR));

//The LOOP part of the REPL 
    while(1){
        write(STDOUT_FILENO, P, strlen(P));
        read_count = read(STDIN_FILENO, buffer, MAXLENBUFFER-1);
        buffer[read_count] = '\0'; 
//Initially, this block was missing, causing execution failure.
//We have to replace the ('\n') with the null..
            buffer[read_count - 1] = '\0';
        if (buffer[read_count - 1] == '\n') {
            buffer[read_count - 1] = '\0'; 
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
        }
    }
    
    return EXIT_SUCCESS;
}

