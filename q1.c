#include <unistd.h>
#include <string.h>
#include <stdlib.h>

//Magic numbers are forbidden... so are the strings!

#define FIRSTSTR "Bienvenue dans le Shell ENSEA.\n"
#define SECONDSTR "Pour quitter, tapez 'exit'.\n"
#define P "enseash % "

int main(int argc, char **argv)
{
    write(STDOUT_FILENO, FIRSTSTR, strlen(FIRSTSTR));
    write(STDOUT_FILENO, SECONDSTR, strlen(SECONDSTR));
    write(STDOUT_FILENO, P, strlen(P));
    return EXIT_SUCCESS;
}
