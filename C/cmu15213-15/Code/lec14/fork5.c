#include <stdio.h>
#include <sys/wait.h>
#include "fork_wrapper.h"

int main(void){
    int child_status;
    pid_t wpid = wait(&child_status);
    if (WIFEXITED(child_status))
        printf("Child %d terminated with exit status %d\n",
    wpid, WEXITSTATUS(child_status));
    else
        printf("Child %d terminate abnormally\n", wpid);
}