#include <stdio.h>
#include "fork_wrapper.h"

int main(void){
    int pid;
    pid = Fork();
    char *label;
    if (pid == 0){
        label = "Child";
    } else {
        label = "Parent";
    }
    printf("%s PID: %i\n", label, getpid());
    printf("%s PPID: %i\n", label, getppid());
}