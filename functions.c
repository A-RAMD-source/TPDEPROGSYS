#include "enseash.h"

void display_welcome() {
    write(STDOUT_FILENO, FIRSTSTR, strlen(FIRSTSTR));
    write(STDOUT_FILENO, SECONDSTR, strlen(SECONDSTR));
}

long calculate_ms(struct timespec startingtime, struct timespec endtime) {
    return (endtime.tv_sec - startingtime.tv_sec) * 1000 + (endtime.tv_nsec - startingtime.tv_nsec) / 1000000;
}

void update_prompt_status(char *prompt_status, int status, long elapsedtimeinms) {
    if (WIFEXITED(status)) {
        int exitcode = WEXITSTATUS(status);
        snprintf(prompt_status, MAXSTATUSLEN, "[exit:%d|%ldms] %% ", exitcode, elapsedtimeinms);
    } else if (WIFSIGNALED(status)) {
        int exitsig = WTERMSIG(status);
        snprintf(prompt_status, MAXSTATUSLEN, "[exit:%d|%ldms]%% ", exitsig, elapsedtimeinms);
    }
}
