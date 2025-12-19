#include "enseash.h"

int main(int argc, char **argv)
{
    char buffer[MAXLENBUFFER];
    ssize_t read_count;
    pid_t pid;
    int status;
    struct timespec startingtime, endtime;
    long elapsedtimeinms; 
    char prompt_status[MAXSTATUSLEN] = " % ";
    
    display_welcome();

    while(1){
        write(STDOUT_FILENO, PROMPT_BASE, strlen(PROMPT_BASE));
        write(STDOUT_FILENO, prompt_status, strlen(prompt_status));
        read_count = read(STDIN_FILENO, buffer, MAXLENBUFFER-1);

        if (read_count == 0){
            break;
        }
        buffer[read_count] = '\0';

        if (read_count > 0 && buffer[read_count - 1] == '\n') {
            buffer[read_count - 1] = '\0';
        }
        if (strcmp(buffer, EXIT) == 0){
            break;
        }

        clock_gettime(CLOCK_REALTIME, &startingtime);
        pid = fork(); 

        if (pid == -1){
            perror("fork error");
            continue;
        }
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
    write(STDOUT_FILENO, BYE, strlen(BYE));
    return EXIT_SUCCESS;
}
