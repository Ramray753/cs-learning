#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <sys/syscall.h>

void sigtstp_handler(int);
void unix_error(const char *msg);

int main()
{
    /* Install the SIGTSTP handler */
    if (signal(SIGTSTP, sigtstp_handler) == SIG_ERR)
        unix_error("Installing handler failed!");
    printf("Wait for signal...\n");
    sigset_t mask, prev_mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    /* Block SIGINT and save previous blocked set */
    sigprocmask(SIG_BLOCK, &mask, &prev_mask);
    pause();
    printf("Done!\n");
}

void sigtstp_handler(int signum){
    printf("Caught signal: %d\n", signum);
    for (int i = 0; i < 3; i++){
        printf("Sleep %i\n", i);
        sleep(1);
    }
    printf("OK...Let you go\n");
}

void unix_error(const char *msg)
{
    int errnum = errno;
    fprintf(stderr, "%s (%d: %s)\n", msg, errnum, strerror(errnum));
    exit(EXIT_FAILURE);
}