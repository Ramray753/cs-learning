#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <sys/syscall.h>

void sigint_handler(int);
void sigtstp_handler(int);
void unix_error(const char *msg);

int main()
{
    /* Install the SIGINT handler */
    if (signal(SIGINT, sigint_handler) == SIG_ERR)
        unix_error("Installing handler failed!");
    if (signal(SIGTSTP, sigtstp_handler) == SIG_ERR)
        unix_error("Installing handler failed!");
    printf("T(%li) - Wait for signal...\n", syscall(__NR_gettid));
    pause();
    printf("T(%li) - Done!\n", syscall(__NR_gettid));
}

void sigint_handler(int signum){
    printf("T(%li) - Caught signal: %d\n", syscall(__NR_gettid), signum);
    for (int i = 0; i < 5; i++){
        printf("T(%li) - Sleep %i\n", syscall(__NR_gettid), i);
        sleep(1);
    }
}

void sigtstp_handler(int signum){
    printf("T(%li) - Caught signal: %d\n", syscall(__NR_gettid), signum);
    for (int i = 0; i < 3; i++){
        printf("T(%li) - Sleep %i\n", syscall(__NR_gettid), i);
        sleep(1);
    }
    printf("T(%li) - OK...Let you go\n", syscall(__NR_gettid));
}

void unix_error(const char *msg)
{
    int errnum = errno;
    fprintf(stderr, "%s (%d: %s)\n", msg, errnum, strerror(errnum));
    exit(EXIT_FAILURE);
}