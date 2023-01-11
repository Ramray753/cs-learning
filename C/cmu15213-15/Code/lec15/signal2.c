#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

void sigint_handler(int);
void unix_error(const char *msg);

int main()
{
    /* Install the SIGINT handler */
    if (signal(SIGINT, sigint_handler) == SIG_ERR)
        unix_error("Installing handler failed!");
    printf("Wait for signal...\n");
    raise(SIGINT);
    fprintf(stderr, "Value of errno: %d\n", errno);
    fprintf(stderr, "Error description: %s\n", strerror(errno));
}

void sigint_handler(int signum){
    printf("Caught signal: %d\n", signum);
}

void unix_error(const char *msg)
{
    int errnum = errno;
    fprintf(stderr, "%s (%d: %s)\n", msg, errnum, strerror(errnum));
    exit(EXIT_FAILURE);
}