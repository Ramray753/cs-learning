#include <stdio.h>
#include <sys/wait.h>
#include "fork_wrapper.h"
#define N 10

int main(void){
    pid_t pid[N];
    int i, child_status;
    for (i = 0; i < N; i++)
        if ((pid[i] = Fork()) == 0) {
            exit(100+i); /* Child */
        }
    for (i = N - 1; i >= 0; i--) { /* Parent */
        pid_t wpid = waitpid(pid[i], &child_status, 0);
        if (WIFEXITED(child_status))
            printf("Child %d terminated with exit status %d\n",
        wpid, WEXITSTATUS(child_status));
        else
            printf("Child %d terminate abnormally\n", wpid);
    }
}