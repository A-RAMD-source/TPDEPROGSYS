#ifndef ENSEASH_H
#define ENSEASH_H

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>
#include <time.h>

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

void display_welcome();
long calculate_ms(struct timespec startingtime, struct timespec endtime);
void update_prompt_status(char *prompt_status, int status, long elapsedtimeinms);

#endif
