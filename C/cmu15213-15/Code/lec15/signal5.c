#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <sys/syscall.h>

void sigint_handler(int);
void unix_error(const char *msg);

int main()
{
    /* Install the SIGINT handler */
    if (signal(SIGINT, sigint_handler) == SIG_ERR)
        unix_error("Installing handler failed!");
    printf("Wait for signal...\n");
    for (int i = 0; i < 5; i++){
        printf("Main: sleep %i\n", i);
        sleep(1);
    }
    printf("Done!\n");
}

void sigint_handler(int signum){
    printf("Caught signal: %d\n", signum);
    for (int i = 0; i < 5; i++){
        printf("Handler: sleep %i\n", i);
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